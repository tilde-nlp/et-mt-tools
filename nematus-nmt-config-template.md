# Nematus NMT System Configuration Template

The following template (i.e. a python script that requires paths to be edited) describes the NMT system training set-up that was used to train the NMT systems in scope of the project _EKT63 Eesti masintõlke kvaliteedi parendamine keeleteadmiste abil_.

```
import numpy
import os
import sys

VOCAB_SIZE = 100000
SRC = "[Source_Language_code]"
TGT = "[Target_Language_code]"
DATA_PREFIX = "[Training_Data_Path_With_Prefix]"
VALID_PREFIX = "[Tuning_Data_Path_With_Prefix]"

from nematus.nmt import train

if __name__ == '__main__':
    validerr = train(saveto='[Model_Path].npz',
        reload_=True,
        dim_word=500,
        dim=1024,
        n_words=VOCAB_SIZE,
        n_words_src=VOCAB_SIZE,
        decay_c=0.,
        clip_c=1.,
        lrate=0.0001,
        optimizer='adadelta',
        maxlen=50,
        batch_size=80,
        valid_batch_size=80,
        datasets=[DATA_PREFIX + '.bpe.' + SRC, DATA_PREFIX + '.bpe.' + TGT],
        valid_datasets=[VALID_PREFIX + '.bpe.' + SRC, VALID_PREFIX + '.bpe.' + TGT],
        dictionaries=[DATA_PREFIX + '.bpe.' + SRC + '.json',DATA_PREFIX + '.bpe.' + TGT + '.json'],
        validFreq=10000,
        dispFreq=1000,
        saveFreq=30000,
        sampleFreq=10000,
        use_dropout=False,
        dropout_embedding=0.2, # dropout for input embeddings (0: no dropout)
        dropout_hidden=0.2, # dropout for hidden layers (0: no dropout)
        dropout_source=0.1, # dropout source words (0: no dropout)
        dropout_target=0.1, # dropout target words (0: no dropout)
        overwrite=False,
        external_validation_script='[Validation_Script_Path]')
    print validerr
```

The parameters `[Source_Language_code]`, `[Target_Language_code]`, `[Training_Data_Path_With_Prefix]`, `[Tuning_Data_Path_With_Prefix]`, `[Model_Path]`, and `[Validation_Script_Path]` have to be replaced with precise values!