
gaudi_add_module(MyProcessor
                 SOURCES src/MyProcessor.cc
                 LINK k4FWCore::k4FWCore 
                      Gaudi::GaudiKernel 
                      Gaudi::GaudiAlgLib 
                      ${CLHEP_LIBRARIES} 
                      ${GSL_LIBRARIES}
                      ${LCIO_LIBRARIES}
                      EDM4HEP::edm4hep EDM4HEP::edm4hepDict
                      ${ROOT_LIBRARIES}
)

target_include_directories(MyProcessor
  PUBLIC ${LCIO_INCLUDE_DIRS})

install(TARGETS MyProcessor
  EXPORT CEPCSWTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev)
