# Estonian Compound Word Splitting for SMT

This document describes the SMT system developement scenarios with the compound splitting tools provided in the **bEstMT-tools** repository that have been developed within the project **EKT63 Eesti masintõlke kvaliteedi parendamine keeleteadmiste abil** (further named as **bEstMT**) of the program **Riiklik programm "Eesti keeletehnoloogia (2011-2017)**.

##The Simple System Scenario

The simplest scenario that ensures that compound words will be translated (*as long as their comprising words are present in the parallel training data*) is to split all compound words prior to SMT system training on the source side of the training data as well as in the source text prior to translation.

> **Note:**
> It is clear (and experiment results from the bEstMT project support this) that splitting all compound words may not be productive as there are frequently occurring compound words in the training data and we loose important information when splitting up compound words (especially if the compound words in a split form comprise a phrase of a different meaning). This issue is addressed further down. However, we will ignore this issue for now to show how to apply the compound splitting tools.

To train an SMT system that handles split compounds, you need to:

 1. Perform SMT system training (with the [Moses EMS](http://www.statmt.org/moses/?n=FactoredTraining.EMS) system) up to and including the corpus tokenisation step.
 2. Split compound words on the source side of the tokenised training data, tuning data, and evaluation data with [et_split_compounds](CompoundSplitter).
 3. Adjust the Moses configuration file of the SMT system so that the newly created training data would be accepted (the parameters are: `tokenized-stem` under `CORPUS`, and `tokenized-input` under `TUNING` and `EVALUATION`).
 4. Continue training of the SMT system.

> **Note:**
> For the training to work, you will need to create copies of the target language files (training data, tuning data, and evaluation data) as the SMT system training script will assume that the names of the source and target files without the file extension (i.e., the language code) have to match. 

When training is completed, you have to make sure that prior to translation, the same compound splitting method is applied right after tokenisation as used during training of the SMT system.

Now, let us look at the issue that **not all compound words should be split**. It is important to either identify frequently occurring compound words or to split up only rarely occurring compound words or out-of-vocabulary compound words. This is what the [Moses integrated statistical compound splitting tool](https://github.com/moses-smt/mosesdecoder/blob/master/scripts/generic/compound-splitter.perl) does (it splits only rarely occurring and out-of-vocabulary compound words).

In this repository we do not provide tools for identification of compound words that should not be split. However, this can be achieved by analysing the source language training data and creating a file containing all words that occur a fixed minimum amount of times. For the implementation, you can take the source code of the tool [ProcessUnknownWords](CompoundWordProcessingTools) and adapt it so that instead of an unknown word file it takes in input the list of the words that should not be split.

##Two System Scenario

A different scenario is to train two SMT systems and translate sentences in two steps. The first SMT system can be trained on non-split training data. For the second SMT system, we have identified that the best results can be achieved if we:

 1. Perform SMT system training (with the [Moses EMS](http://www.statmt.org/moses/?n=FactoredTraining.EMS) system) up to and including the corpus tokenisation step.
 2. Split compound words on the source side of the tokenised training data, tuning data, and evaluation data with [et_split_compounds](CompoundSplitter).
 3. Concatenate the non-split and the split tokenised training data and tuning data so that the training data and tuning data would contain compound words in both non-split and split forms.
 4. Adjust the Moses configuration file of the SMT system so that the newly created training data would be used for training (the parameters are: `tokenized-stem` under `CORPUS`, and `tokenized-input` under `TUNING` and `EVALUATION`).
 5. Continue training of the SMT system.

The translation can then be performed as follows:

 1. Translate each sentence with the first SMT system using Moses with the `-output-unknowns [UNKFile]` parameter. This will create a file containing unknown words.
 2. Then split the unknown words in the source sentences that are compound words with the tool [ProcessUnknownWords](CompoundWordProcessingTools).
 3. Translate the split sentences with the second SMT system.

If you want to evaluate this scenario, we have created the tool [GetCorrectEvalData](CompoundWordProcessingTools) that allows you to create SMT output data for evaluation that simulates this two step translation scenario when you translate all sentences with the first SMT system, execute the compound splitting for unknown words, and translate all sentences with the second SMT system. So, basically, you do not have to pick out the sentences that were translated twice and you can translate all sentences twice and the tool will figure out which sentences should have been translated once (i.e., they did not contain unknown compound words) and which sentences should have been translated twice (i.e., they contained unknown compound words).

> **Note:**
> This two-pass (or two-step) translation scenario has shown to achieve the best results for Estonian-English in our experiments. 

##Back-off Scenario

A different solution  is to create **two translation models** in a **back-off scenario** for the SMT system such that the primary translation model contains phrase translations in non-split forms and the back-off model contains phrases where compound words on the source side are split.

To create such a system:

 1. Train two SMT systems (e.g., using the guidelines provided in the previous section).
 2. Create a new `moses.ini` file (e.g., a copy of the `moses.ini` file of the second SMT system) that lists both translation models in a back-off scenario, e.g.:
 
 ```ini
 [mapping]
 0 T 0
 1 T 1
 
 [decoding-graph-backoff]
 0
 1
 
 [ttable-file]
 12 0 0 5 /home/user/primary-table-bin.1
 12 0 0 5 /home/user/secondary-table-bin.1
 ```
 > **Note:**
 > This is just an example of the parts in the `moses.ini` file that need to be adjusted. It may differ depending on your SMT system training setup.
 
 3. Create a copy of the EMS configuration file `config` of the second system (where compound words in tuning data are both in split and non-split forms) and specify the path of the newly created `moses.ini` file under `[TRAINING]`.
 4. Continue training of the SMT system using the newly created configuration file.

Then, for translation, two scenarios can be applied:

1. One-pass (or one step) translation. Similarly to the Moses compound splitting tool, we would then need a dictionary that tells us which compound words should not be split. When translating, the split compound translations would be selected from the back-off translation model if they will not be present in the primary translation model.
2. Two-pass (or two step) translation. First, we translate each sentence with compound words in a non-split form using Moses with the `-output-unknowns [UNKFile]` parameter (the translations of words/phrases will be mostly selected from the primary translation model as that will be prioritised). Then, we split unknown compound words in each sentence using the unknown word file `[UNKFile]` produced by Moses. Finally, we translate the sentence again with the SMT system. This time, translations of the split compound words will be selected from the back-off model (if they will not be present in the primary translation model).

> **Note:**
> In order to limit the selection of translations for split compound parts from the primary translation model, you can use a compound splitting option that marks the splitting points with the `γ` symbol. 

