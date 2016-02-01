# Estonian Compound Word Splitting for SMT

Work In Progress!!! Read at your own risk

This document describes the SMT system developement scenarios with the compound splitting tools provided in the **bEstMT-tools** repository.

There are many scenarios in which the tools could be applied, therefore, in this document we limit the scenarios to:

- Training of one SMT system and translation of sentences once
- Training of two SMT systems (or one SMT system with a back-off model) and translation of sentences twice (using a two-pass translation approach)

The second approach has showed the best results in our experiments when translating from Estonian to English.

##One System Scenario
The simplest scenario that ensures that compound words will be translated (as long as their comprising words are present in the parallel training data) is to split all compound words prior to SMT system training on the source side of the training data as well as in the source text prior to translation. It is clear (and experiment results from the bEstMT project support this) that splitting all compound words may not be productive as there are frequently occurring compound words in the training data and we loose important information when splitting up compound words (especially if the compound words in a split form comprise a phrase of a different meaning). This issue is addressed further down. However, we will ignore this issue in this section to show how to apply the compound splitting tools.


Therefore, it could be important to either identify frequently occurring compound words or to split up only rarely occurring compound words or out-of-vocabulary compound words. This is what the [Moses integrated statistical compound splitting tool](https://github.com/moses-smt/mosesdecoder/blob/master/scripts/generic/compound-splitter.perl) does (it splits only rarely occurring and out-of-vocabulary compound words).
Having said that, it is possible to create two translation models in a back-off scenario for the SMT system such that the primary translation model contains phrase translations in non-split form and the back-off model contains phrases in split form. Similarly to the Moses compound splitting tool, we would then need a dictionary that tells us which compound words should not be split. When translating, the split compound translations would be selected from the back-off translation model if they will not be present in the primary translation model.

To create such an SMT system, we would need to 


##Two System Scenario

Tools developed within the scope of the bEstMT project

```flow
st=>start: Start
e=>end
op=>operation: My Operation
cond=>condition: Yes or No?

st->op->cond
cond(yes)->e
cond(no)->op
```
