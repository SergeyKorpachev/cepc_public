#!/usr/bin/env python

import os
import sys
import glob
from Gaudi.Configuration import *

RECO_DIR = "/cefs/higgs/zhangkl/Production/25036/E240_eeHX/Reco"

FILE_NUMBER = "00000"

FILE_PATH = f"{RECO_DIR}/rec_E240_eeHX_{FILE_NUMBER}.root"

if not os.path.exists(FILE_PATH):
    print(f"ERROR: File not found: {FILE_PATH}")
    sys.exit(1)

print(f"Processing: {FILE_PATH}")
print("=" * 60)

from Configurables import myChecking
checker = myChecking("myChecking")

checker.printInterval = 1000
checker.printAllPDG = True

from Configurables import k4DataSvc
podioevent = k4DataSvc("EventDataSvc", input=FILE_PATH)

from Configurables import PodioInput
inp = PodioInput("InputReader")
inp.collections = ["CyberPFOPID", "MCParticle"]

from Configurables import ApplicationMgr
ApplicationMgr(
    TopAlg=[inp, checker],
    EvtSel="NONE",
    EvtMax=-1,
    ExtSvc=[podioevent],
    OutputLevel=3,
)
