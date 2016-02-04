#Estonian Machine Translation Tools

This repository contains different linguistic processing tools and guidelines for statistical machine translation (SMT) system training developed within the project **EKT63 Eesti masintõlke kvaliteedi parendamine keeleteadmiste abil** of the program **Riiklik programm "Eesti keeletehnoloogia (2011-2017)**.

During the project, multiple new technologies were investigated for the improvement of SMT system quality when translating from and to the Estonian language. The scientific activity reports are available as the project's deliverables. This repository lists the publicly available components developed within the project.

The tools have been developed for the Moses SMT system.

##Repository Structure

The repository contains the following tools:

- Estonian Morphological Analysis Based Compound Splitter - [et_split_compounds](CompoundSplitter) (C++)
- Splitting tool for words containing dash symbols - [SplitDashWordsForMoses](CompoundWordProcessingTools) (C#)
- Unknown word processing tool for two-pass translation [ProcessUnknownWords](CompoundWordProcessingTools) (C#)
- Evaluation data creator for two-pass translation scenarios [GetCorrectEvalData](CompoundWordProcessingTools) (C#)

Further information regarding each of the tools (including execution instructions) can be acquired by following the links above.

SMT system development scenarios for the experiments with compound splitting are described [here](CompoundSplittingScenarios.md).

SMT system development scenarios for the experiments with phrase table triangulation are described [here](PhraseTableTriangulationScenarios.md).