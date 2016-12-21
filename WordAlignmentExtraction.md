# Word Alignment Extraction from Alignment Matrices

In order to integrate NMT systems within existing MT infrastructures, it may be necessary to obtain word alignment information from the NMT system's attention mechanism. The alignments in the MT infrastructure can be used to correctly process formatting tags when translating documents (including web pages) and to visualise the translations in graphical user interfaces. The word alignment extraction methodology published in the following paper was used as the basis for further improvements in the project.

```
@inproceedings{Pinnis2016,
	address = {Riga, Latvia},
	author = {Pinnis, Mārcis},
	booktitle = {Human Language Technologies – The Baltic Perspective - Proceedings of the Seventh International Conference Baltic HLT 2016},
	doi = {10.3233/978-1-61499-701-6-84},
	isbn = {9781614997016},
	keywords = {english,hybrid system,latvian,neural machine translation},
	pages = {84--91},
	publisher = {IOS Press},
	title = {{Towards Hybrid Neural Machine Translation for English-Latvian}},
	year = {2016}
}
```

For the integration we selected one out of the four different word alignment extraction methods that were published in Pinnis (2016) and created a tool that can read an alignment matrix produced by the [modified AmuNMT decoder](ImprovementsToTheAmuNMTDecoder.md) and produce word alignments that can be used in the existing MT workflows for document translation and formatting tag processing. The method has been further improved to handle low confidence (miss)-alignments better.

The algorithm to acquire flat word alignments from the alignment matrix operates as follows (Pinnis, 2016):

1. First, we identify cells in the alignment matrix that correspond to maximum source-to-target and target-to-source alignments (i.e. we find those source and target token pairs for which the alignment is unambiguous).
2. Then, we perform a leftward and rightward search around each identified unambiguous target token to identify additional source tokens for which the maximum alignment weight is assigned to the target token (i.e. we expand the previously identified source and target token pairs with unambiguous many-to-one alignments). The leftward and rightward search is performed according to the scenario `without gaps` as described in Pinnis (2016). However, different from the original implementation, we limited the leftward and rightward search with an alignment probability threshold so that tokens that are paired to the main token with a gap in-between would not be paired if the system's confidence is too low.
3. Next, among the remaining unpaired target and source tokens, we identify the respective source and target tokens with the highest alignment weights. This is the first ambiguous step as this alignment will not be reciprocal (i.e. for a yet unpaired target token, if we identify that a particular unpaired source token has the highest alignment weight, then the maximum alignment weight for the source token is with a different token).
4. Next, the second step is performed for the target tokens that were paired in the third step. This allows capturing possibly missing many-to-one alignments.

The fifth step (linking of unpaired low-confidence source tokens to target tokens) is not performed as low-confidence alignments increase miss-alignment errors and our test experiments have shown that these tokens are most often functional words and it is safe to leave such tokens unpaired (at least in our MT workflows).

The collection of tools for alignment extraction from NMT attention-based alignment matrices and smarter processing of unknown words in factored models can be found [here](https://github.com/tilde-nlp/neural-machine-translation-tools).