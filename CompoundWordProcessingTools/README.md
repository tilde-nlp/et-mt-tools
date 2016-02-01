#Compound Word Processing Tools

The Estonian Compound Word Processing Tools have been developed within the project **Riiklik programm "Eesti keeletehnoloogia (2011-2017)** (further named as **bEstMT**).

The toolkit consists of three tools:

- Splitting tool for words containing dash symbols (**SplitDashWordsForMoses**) - this tool allows splitting compound words that contain hyphen symbols in SMT system training data. It is assumed that the input data will be in a normalised form (i.e., only the *hyphen-minus* symbol will be present in the data). This tool has been created, because the morphological analysis-based compound splitting tool will not split compound words that contain dash symbols. This tool allows further lowering out-of-vocabulary word rates (if applied both on SMT system training data and the source text prior to translation).
- Unknown word processing tool for two-pass translation (**ProcessUnknownWords**) - this tool allows splitting compound words in source text prior to translation with Moses provided that an unknown word file is given. The unknown word file can be either created by Moses (i.e., by translating the source text with Moses using the `-output-unknowns` parameter) or by a different tool (that has to be created by the reader) that reads a dictionary of known words that should not be split and writes in output words that should be split by the *ProcessUnknownWords* tool. The file has to be in the same format as the unknown word log file created by the Moses decoder.
- Evaluation data creator for two-pass translation scenarios (**GetCorrectEvalData**) - this tool allows creating evaluation data that simulates a two-pass (or two step) translation scenario where: 1) first, sentences are translated without compound splitting, and 2) sentences, which contain unknown words that are compound words, are processed in a second pass by splitting the unknown compound words and translated with either a second SMT system or the same SMT system (depending on the scenario) a second time. To create the evaluation data, all sentences can be translated two times and the correct evaluation data (i.e., the data that simulates the two-pass scenario) is prepared using both translation pass outputs and the output of the *ProcessUnknownWords* tool.

##Build Instructions

All three tools have been bundled in a Microsoft Visual Studio 2013 solution. To build the tools, open the `CompoundWordProcessingTools.sln` file in Visual Studio 2013 (or newer) and build the solution. The tools will be compiled in the `Release` directory.

##Execution Instructions

The **SplitDashWordsForMoses** tool can be executed using the following command line:

```
.\SplitDashWordsForMoses.exe -i [InputFile] -o [OutputFile] (-a)
```

The optional `-a` parameter indicates that splitting points of words have to be marked with `γ` symbols.

The **ProcessUnknownWords** tool can be executed using the following command line:

```
.\ProcessUnknownWords.exe -u [UNKFile] -i [SourceFile] -o [OutputFile] -scp [CompoundSplitterPath] -sca [CompoundSplitterParameters] (-s)
```

The parameters are as follows:

- `-u [UNKFile]` - the path to the unknown word log file produced by Moses in the first translation attempt (or step). The unknown word log file can be produced by executing Moses with the `-output-unknowns [UNKFile]` parameter.
-  `-i [SourceFile]` - the path to the source file that was translated with Moses in the first translation attempt.
-  `-o [OutputFile]` - the path to the processed source file (i.e., the output file) with compound words split.
-  `-scp [CompoundSplitterPath]` - the full path to the compound splitting tool `et_split_compounds`.
-  `-sca [CompoundSplitterParameters]` - the parameters of the compound splitting tool (i.e., `[DictionaryPath] [Configuration]` parameters of the `et_split_compounds` tool).
-  `-s` - an optional parameter that indicates (if present) that words containing dash symbols have to be split. If the compound splitting parameters will require to mark splitting points with gamma symbols, dashes containing word splitting points will be also marked with dashes.

The **GetCorrectEvalData** tool can be executed using the following command line:

```
.\GetCorrectEvalData.exe -u [ProcFile] -i1 [FirstPassOutputFile] -i2 [SecondPassOutputFile] -o [OutputFile]
```

The parameters are as follows:

- `-p [ProcFile]` - the path to the `*.proc` file produced by `ProcessUnknownWords.exe` after the first translation pass. The file contains `1` on lines in which Moses identified unknown words that could be split by the `ProcessUnknownWords.exe` tool.
-  `-i1 [FirstPassOutputFile]` - the path to the output file of the first translation pass (i.e., the translation with the SMT system without split compounds).
-  `-i2 [SecondPassOutputFile]` - the path to the output file of the second translation pass (i.e., the translation with the SMT system with split compounds).
-  `-o [OutputFile]` - the path to the output file where sentences are selected from the first pass if they do not contain unknown words that can be split by `ProcessUnknownWords.exe` and from the second pass if they contain such words.

> **Note:**
> The input data for all tools should be encoded in UTF-8 without the BOM marker. Otherwise, the compound splitter will not perform correctly.
