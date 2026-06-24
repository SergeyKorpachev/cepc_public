#!/usr/bin/env python

from Gaudi.Configuration import *

from Configurables import k4DataSvc
dsvc = k4DataSvc("EventDataSvc")

# read LCIO files
from Configurables import LCIOInput
read = LCIOInput("read")
read.inputs = [
"/cefs/higgs/esvasenin/whizard/hz_WW_4j_240.slcio"
]
read.collections = [
        "MCParticle:MCParticle"
]

from Configurables import TotalInvJets
totalinvjets = TotalInvJets("TotalInvJets")
totalinvjets.TreeOutputFile = "out_totalinvjets_mcparticle.root"

# output
#from Configurables import PodioOutput
#out = PodioOutput("outputalg")
#out.filename = "totalinvjets.root"
#out.outputCommands = ["keep *"]

# ApplicationMgr
from Configurables import ApplicationMgr
mgr = ApplicationMgr(
    TopAlg = [read, totalinvjets],
    EvtSel = 'NONE',
    EvtMax = 1000,
    ExtSvc = [dsvc],
    HistogramPersistency = 'ROOT',
    OutputLevel = INFO
)
