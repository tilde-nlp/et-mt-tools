# Phrase Table Triangulation for SMT

This document describes the SMT system developement scenarios with phrase table triangulation that have been analysed within the project **EKT63 Eesti masintõlke kvaliteedi parendamine keeleteadmiste abil** (further named as **bEstMT**) of the program **Riiklik programm "Eesti keeletehnoloogia (2011-2017)**.

Within the scope of the *bEstMT* project, we investigated different phrase table triangulation and merging methods that would allow us to create better quality SMT systems for language pairs for which there are limited amounts of parallel data available. One such language pair is Estonian-Russian. Further we describe how to create SMT systems with triangulated phrase tables with the Moses SMT system.

## Pivot Language

At first, you have to decide, which language will you use as the pivot language. There are two factors to consider: 1) which are the possible pivot languages for which you have the most parallel data (source-to-pivot and pivot-to-target) available, and 2) are the languages similar in terms of morphology and sentence structure to either the source or the target language. If the languages will differ too much in terms of morphology ans sentence structure, you risk loosing information or introducing a lot of noise due to pivoting. Having said that, in the *bEstMT* project, the only viable option was English, because for other potential pivot languages there were simply not enough parallel data available to train acceptable quality SMT systems.

## Source-to-Pivot and Pivot-to-Source Phrase Tables

When the pivot language is identified, create two phrase tables (i.e., you can also train two SMT systems with Moses and just stop the training when the phrase tables have been created) - a source-to-pivot phrase table and a pivot-to-target phrase table.

## Phrase Table Pruning

As the phrase tables will contain a wast proportion of wrong phrase pairs (i.e., if you have ever looked at a phrase table, you will understand what we mean). It is important to filter the phrase tables, because the next steps will be otherwise overwhelmed by the high level of noise.

There are quite a few methods for pruning a phrase table. However, in our experiments we used the method by Johnson et al. (2007). The method is implemented in the [filter-pt](https://github.com/moses-smt/mosesdecoder/tree/master/contrib/relent-filter/sigtest-filter) tool that can be found in the Moses repository.

> H. Johnson, J. Martin, G. Foster and R. Kuhn. (2007) **Improving Translation Quality by Discarding Most of the Phrasetable**. In *Proceedings of the 2007 Joint Conference on Empirical Methods in Natural Language Processing and Computational Natural Language Learning (EMNLP-CoNLL)*, pp. 967-975.

To filter the source-to-pivot (*s-to-p*) and pivot-to-target (*p-to-t*) phrase tables, we used the default suggested settings for the `filterpt` tool. However, for triangulation it may be necessary to limit the maximum number of phrase translations (i.e., the `-n [X]` parameter) even further. An example of the execution commands is as follows:

```bash
./filter-pt -e "[corpus-s-to-p.p]" -f "[Corpus-s-to-p.s]" -l a+e -n 30 \
  < "[s-to-p-phrase-table]" \
  > "[filtered-s-to-p-phrase-table]"
#[corpus-s-to-p.p] - the path to the pivot language training data file that was used to create the s-to-p phrase table ([s-to-p-phrase-table])

./filter-pt -e "[Corpus-p-to-t.t]" -f "[Corpus-p-to-t.p]" -l a+e -n 30 \
  < "[p-to-t-phrase-table]" \
  > "[filtered-p-to-t-phrase-table]"
```

## Phrase Table Triangulation

The next step after creation and pruning of phrase tables is the phrase table triangulation. For phrase table triangulation in our experiments, we applied the method by Hoang & Bojar (2015) that is implemented in the [TmTriangulate](https://github.com/tamhd/MultiMT) tool that can be found also on GitHub.

> Hoang, D. T., & Bojar, O. (2015). **TmTriangulate: A Tool for Phrase Table Triangulation**. *The Prague Bulletin of Mathematical Linguistics*, 104(1), 75-86.

An example of the `tmtriangulate` execution command is as follows:

```bash
python tmtriangulate.py features_based -m sppt \
  -s "[filtered-s-to-p-phrase-table]" \
  -t "[filtered-p-to-t-phrase-table]" \
  -o "[triangulated-s-to-t-phrase-table]" \
  -tmpdir "[temporary-file-directory]"
  
  #Note that the [...] should be replaced with full paths to files or directories.
  #You may also want to create the lex files for the triangulated phrase tables
  #  by adding the -l parameter (the lex files will be useful in further steps).
```

## Phrase Table Merging

Within the scope of the *bEstMT* project we analysed the performance of SMT systems using phrase tables trained on direct source-to-target parallel corpora, just the triangulated phrase table (which obviously achieved significantly lower scores), and phrase tables that were acquired using different phrase table merging techniques. In our experiments we investigated:

- the method by Sennrich (2012) that is implemented in the [tmcombine](https://github.com/moses-smt/mosesdecoder/tree/master/contrib/tmcombine) tool that is available from the Moses repository.

 > Sennrich, R. (2012). **Perplexity Minimization for Translation Model Domain Adaptation in Statistical Machine Translation**. *Proceedings of the 13th Conference of the European Chapter of the Association for Computational Linguistics (EACL 2012)*, 539–549.
 
 The `tmcombine` tool requires a source-to-target phrase table as tuning data. Such a "*tuning*" phrase table can can be acquired if you build a phrase table using just the tuning data set of the source-to-target SMT system. An example of the execution command is as follows (note that there are many more options that can be configured, here we provide just a basic example):

```bash
 python tmcombine.py "combine_given_tuning_set" \
  "[PrimaryPhraseTableDirectory]" \
  "[SecondaryPhraseTableDirectory]" \
  -o "[MergedPhraseTablePath]" -r "[TuningPhraseTablePath]"
 
  #Note that the directory structure where the primary an secondary phrase table
  # are located is such that the phrase tables and the lex files have to be 
  # stored in the following paths:
  # [Primary/SecondaryPhraseTableDirectory]/model/phrase-table
  # [Primary/SecondaryPhraseTableDirectory]/model/lex.e2f
  # [Primary/SecondaryPhraseTableDirectory]/model/lex.f2e
  #The output and tuning phrase table paths are absolute paths
  # to the phrase table files
```

- the fill-up and back-off methods by Bisazza et al. (2011) that are implemented in the [combine-ptables](https://github.com/moses-smt/mosesdecoder/tree/master/contrib/combine-ptables) tool that is available from the Moses repository.

 > Bisazza, A., Ruiz, N., Federico, M., & Kessler, F. F. B. (2011, December). **Fill-up versus interpolation methods for phrase-based SMT adaptation**. In *IWSLT* (pp. 136-143).

An example of the execution command of the `combine-ptables` tool is as follows:

```bash
#Fill-up method:
perl combine-ptables.pl --mode=fillup "[PrimaryPhraseTablePath]" \
  "[SecondaryPhraseTablePath]" > "[MergedPhraseTablePath]"

#Back-off method:
perl combine-ptables.pl --mode=backoff "[PrimaryPhraseTablePath]" \
  "[SecondaryPhraseTablePath]" > "[MergedPhraseTablePath]"

#Note that the phrase table paths are absolute paths to the phrase table files.
```

##Results

The following results show that the triangulation methods in combination with the phrase table merging methods allow improving SMT quality over the baseline system. However, a better result can be achieved by performing just pivot translation using the two SMT systems whose phrase tables were used to create the triangulated phrase table.

| Scenario                                       | METEOR     | TER        | BLEU           | NIST              |
| ---------------------------------------------- | ----------:| ----------:| --------------:| ----------------: |
| Baseline (et-ru)                               | 0.3207     | 0.9801     | 8.50±0.91      | 3.6182±0.1562     |
| `tmtriangulate` (et-en-ru)                     | 0.3082     | 0.9890     | 6.88±0.92      | 3.2882±0.1569     |
| `tmcombine` (et-ru + et-en-ru)                 | 0.3175     | 0.9835     | 9.06±1.14      | 3.8137±0.1696     |
| `combine-ptables` fill-up (et-ru + et-en-ru)   | 0.3128     | **0.9579** | 9.25±1.06      | 3.7478±0.1725     |
| `combine-ptables` back-off ( et-ru + et-en-ru) | 0.3010     | 0.9674     | 8.75±1.06      | 3.7262±0.1795     |
| Pivot translation (et-en + en-ru)              | **0.3318** | 1.0422     | **11.02±1.34** | **4.2333±0.1956** |
