# Estonian Machine Translation Tools

This repository contains different linguistic processing tools and guidelines for statistical machine translation (SMT) system training developed within the project **[EKT63 Eesti masintõlke kvaliteedi parendamine keeleteadmiste abil](https://www.keeletehnoloogia.ee/en/projects-2011-2017/linguistic-knowledge-in-estonian-machine-translation/linguistic-knowledge-in-estonian-machine-translation?set_language=en)** of the program **Riiklik programm "Eesti keeletehnoloogia (2011-2017)**.

During the project, multiple new technologies were investigated for the improvement of SMT system quality when translating from and to the Estonian language. The scientific activity reports are available as the project's deliverables. This repository lists the publicly available components developed within the project.

The tools have been developed for the Moses SMT system, the Nematus NMT system (for NMT system training) and the AmuNMT decoder (for translation with the NMT models trained with Nematus).

## Project period of 2015

The repository contains the following tools from the project's period of 2015:

- Estonian Morphological Analysis Based Compound Splitter - [et_split_compounds](CompoundSplitter) (C++)
- Splitting tool for words containing dash symbols - [SplitDashWordsForMoses](CompoundWordProcessingTools) (C#)
- Unknown word processing tool for two-pass translation [ProcessUnknownWords](CompoundWordProcessingTools) (C#)
- Evaluation data creator for two-pass translation scenarios [GetCorrectEvalData](CompoundWordProcessingTools) (C#)

Further information regarding each of the tools (including execution instructions) can be acquired by following the links above.

SMT system development scenarios for the experiments with compound splitting are described [here](CompoundSplittingScenarios.md).

SMT system development scenarios for the experiments with phrase table triangulation are described [here](PhraseTableTriangulationScenarios.md).

## Project period of 2016

The repository contains the following tools and resources from the project's period of 2016:

- A modified [fork of the AmuNMT](https://github.com/tilde-nlp/amunmt) decoder that allows extracting word alignment matrices and provides better handling of unknown words. The improvements to the AmuNMT decoder have been described [here](ImprovementsToTheAmuNMTDecoder.md).
- Tools for [_flat_ word alignment extraction](https://github.com/tilde-nlp/neural-machine-translation-tools) from word alignment matrices that can be used for NMT system integration in SMT system workflows for document translation, smart handling of formatting tags and visualisation of alignments. More details can be found [here](WordAlignmentExtraction.md).
- A set of regular expression-based pre-processing, normalisation, tokenisation, and post-processing rules that improve word alignment of non-translatable identifiers and numerals for all languages. The rules provide special cases for handling of Estonian - [Improvements to Word Alignment and Tokenisation of Estonian Texts](EstonianTokenisationImprovements.md).
- A sample configuration template that describes the [Nematus](https://github.com/rsennrich/nematus) NMT system training set-up for the English-Estonian-English and Russian-Estonian-Russian systems within the project can be found in [nematus-nmt-config-template.md](nematus-nmt-config-template.md).

## Project period of 2017

The repository contains the following tools and resources from the project's period of 2017:

- Tilde Tõlge [Android app](https://github.com/tilde-nlp/et-mt-tools/tree/master/TildeTolge) with the capability to translate selected text from any other application where text is selectable.