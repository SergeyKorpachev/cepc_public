#!/usr/bin/env python

from Gaudi.Configuration import *

from Configurables import k4DataSvc
dsvc = k4DataSvc("EventDataSvc", input="/cefs/higgs/liugeliang/CEPC/202501/Production/Hinvi/E240_e1e1Hinvi/Combined/rec_E240_e1e1Hinvi_00001.root")

from Configurables import PodioInput
podioinput = PodioInput("PodioReader", collections=[
    "MCParticle"
    ])

from Configurables import MyProcessor
my_proc = MyProcessor("MyProcessor")
my_proc.TreeOutputFile = "out_myprocessor_mc.root"

# output
#from Configurables import PodioOutput
#out = PodioOutput("outputalg")
#out.filename = "out.root"
#out.outputCommands = ["keep *"]

# ApplicationMgr
from Configurables import ApplicationMgr
mgr = ApplicationMgr(
    TopAlg = [podioinput, my_proc],
    EvtSel = 'NONE',
    EvtMax = 100,
    ExtSvc = [dsvc],
    HistogramPersistency = 'ROOT',
    OutputLevel = INFO
)
