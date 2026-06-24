#!/usr/bin/env python

from Gaudi.Configuration import *

from Configurables import k4DataSvc
dsvc = k4DataSvc("EventDataSvc", input="./roots/Rec_TDR_o1_v01.root")

from Configurables import PodioInput
podioinput = PodioInput("PodioReader", collections=[
    "CyberPFO"
    ])

from Configurables import MyProcessor
my_proc = MyProcessor("MyProcessor")
my_proc.TreeOutputFile = "out_myprocessor_cyberpfo.root"

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
    EvtMax = 1000,
    ExtSvc = [dsvc],
    HistogramPersistency = 'ROOT',
    OutputLevel = INFO
)
