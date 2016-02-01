#Estonian Morphological Analysis Based Compound Splitter

The Estonian Morphological Analysis Based Compound Splitter (**et_split_compounds**) has been developed within the project **Riiklik programm "Eesti keeletehnoloogia (2011-2017)** (further named as **bEstMT**).

##Build Instructions

The compound splitter depends on the Estonian morphological analyser **vabamorf** that can be cloned from the repository https://github.com/Filosoft/vabamorf. Once the repository is cloned, copy the `etsplit_compounds.cpp` and the `Makefile.etsplit` files of **et_split_compounds** into the base directory of **vabamorf**.

To build **et_split_compounds** on Linux, run:
```
make -f Makefile.etsplit
```
This command will create the `et_split_compounds` executable.

##Execution Instructions

The compound splitter can be executed using the following command line:

```
./et_split_compounds [DictionaryPath] [Configuration] < [InputFile] > [OutputFile]
```

The `[DictionaryPath]` is the path to the dictionary `et.dct` of the **vabamorf** morphological analyser (the relative path is `dct/binary/et.dct`).

There are several configurations to be considered, when executing the compound splitter. The following configurations are available:

- Without a configuration parameter - spaces will be added before each root.
- `-a` - spaces will be added before each root and the splitting points will be marked with a `γ` symbol.
- `-f` - spaces will be added before each root and each root (with possible suffices) except the last root will be replaced with a corresponding lemma (i.e., instead of a root with an ending, we will keep the lemma of the word). The last component of the compound word is kept as is in order to keep the inflected form of the compound word.
- `-af` - spaces will be added before each root and each root (with possible suffices) except the last root will be replaced with a corresponding lemma. The splitting points will be marked with a `γ` symbol.
- `-l` - spaces will be added before each root and each root (with possible suffices) will be replaced with a corresponding lemma.
- `-al` - spaces will be added before each root, each root (with possible suffices) will be replaced with a corresponding lemma, and the splitting points will be marked with a `γ` symbol.

Here is an example of the different compound splitting combinations:

| Parameters | Interpretation             | Example        |
| ---------- | -------------------------- | -------------- |
| \-         | Original word              | paarikümne     |
| No         | `(word )+word`             | paari kümne    |
| `-a`       | `wordγ (wordγ )*γword`     | paariγ γkümne  |
| `-f`       | `(lemma )+word`            | paar kümne     |
| `-af`      | `lemmaγ (γlemmaγ )*γword`  | paarγ γkümne   |
| `-l`       | `(lemma )+lemma`           | paar kümmend   |
| `-al`      | `lemmaγ (γlemmaγ )*γlemma` | paarγ γkümmend |

> **Note:**
> The input data for the compound splitter should be encoded in UTF-8 without the BOM marker. Otherwise, the compound splitter will not perform correctly.

