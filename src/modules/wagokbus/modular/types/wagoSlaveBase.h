/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago1506
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium -
 *************************************************************************/

#ifndef SRC_MODULES_WAGOKBUS_MODULAR_TYPES_WAGOSLAVEBASE_H_
#define SRC_MODULES_WAGOKBUS_MODULAR_TYPES_WAGOSLAVEBASE_H_

#include "../../../../core/io/configFB/io_slave_multi.h"

#define FUNCTION_BLOCK_CTOR_FOR_WAGO_SLAVES(fbclass, type) \
  fbclass(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) : \
  WagoSlaveBase(type, paContainer, scmFBInterfaceSpec, paInstanceNameId)

#define INIT_HANLDLES(noOfBoolInputs, noOfBoolOutputs, noOfAnalogInputs, noOfAnalogOutputs) \
   void initHandles() override {initHandlesBase(noOfBoolInputs, noOfBoolOutputs, noOfAnalogInputs, noOfAnalogOutputs);};

class WagoSlaveBase : public forte::core::io::IOConfigFBMultiSlave {

  public:
    WagoSlaveBase(int paType, forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);

    ~WagoSlaveBase() override = default;

  protected:

    void initHandlesBase(size_t paNumberOfBoolInputs, size_t paNumberOfBoolOutputs, size_t paNumberOfAnalogInputs, size_t paNumberOfAnalogOutputs);

    virtual void initHandles() = 0;

  private:

    static const TForteUInt8 scmSlaveConfigurationIO[];
    static const TForteUInt8 scmSlaveConfigurationIONum;
};

#endif /* SRC_MODULES_WAGOKBUS_MODULAR_TYPES_WAGOSLAVEBASE_H_ */
