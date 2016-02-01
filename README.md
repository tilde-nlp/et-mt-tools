#bEstMT-tools

This repository contains different linguistic processing tools and guidelines for statistical machine translation (SMT) system training developed within the project **Riiklik programm "Eesti keeletehnoloogia (2011-2017)** (further named as **bEstMT**).

During the *bEstMT* project, multiple new technologies were investigated for the improvement of SMT system quality when translating from and to the Estonian language. The scientific activity reports are available as the project's deliverables. This repository lists the publicly available components developed within the project.

The tools have been developed for the Moses SMT system.

##Repository Structure

The repository contains the following tools:

- Estonian Morphological Analysis Based Compound Splitter - [et_split_compounds](CompoundSplitter) (C++)
- Splitting tool for words containing dash symbols - [SplitDashWordsForMoses](CompoundWordProcessingTools) (C#)
- Unknown word processing tool for two-pass translation [ProcessUnknownWords](CompoundWordProcessingTools) (C#)
- Evaluation data creator for two-pass translation scenarios [GetCorrectEvalData](CompoundWordProcessingTools) (C#)

Further information regarding each of the tools (including execution instructions) can be acquired by following the links above.

SMT system training and evaluation scenarios for the experiments with compound splitting are described [here](CompoundSplittingScenarios.md).