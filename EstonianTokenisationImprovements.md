# Improvements to Word Alignment and Tokenisation of Estonian Texts

To address language-specific and language-independent issues, which were identified while performing analysis of tokenised and aligned parallel texts, we improved the set of regular expressions that are used in: (1) pre-processing, normalisation, and tokenisation steps prior to SMT system training and translation, and (2) post-processing of texts after translation with the SMT decoder.

Changes in the regular expressions are as follows:

* The previous tokeniser divided number ranges in two parts (a number and a negative number), which was wrong. This has been fixed in the current rules (ranges are tokenised correctly).
* A set of Estonian endings is defined to process numerals hyphenated endings. The hyphen and the ending are now tokenised as a single token. For example, the string `30-ga` is tokenised as `30 -ga` (there is a space between the digit “0” and the hyphen symbol).
* A set of units of measurement are now tokenised as a single token, for example, `mg/m2`.
* People tend to use punctuation and special symbols inconsistently (e.g., different types of quotation marks, apostrophes, hyphen symbols, etc.). Quotation marks and apostrophes have been already handled correctly by pre-processing and post-processing rules. However, hyphen symbols required special attention. Therefore, not only the hyphen symbol, but also the dash symbol is now interpreted as a minus sign before a digit.
* Latin-1 and Cyrillic upper-case letters are included in the set of upper-case letters in order to allow such letters in IDs.
* Digits are no longer separated from the beginnings of IDs, for example, `1A2` will remain as a single token.
* IDs with hyphens are kept as single tokens, for example, `rhBMP-2` will remain as a single token.

The rules are stored in the file [tokenisation-rules.txt](tokenisation-rules.txt). For parsing of the rules, a proprietary parser is required. However the regular expressions alone may allow MT developers to improve their tokenisation tools. The regular expressions can also be useful in many other natural language processing applications.
