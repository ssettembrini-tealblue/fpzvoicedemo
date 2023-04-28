#include "bluetooth_services_model.h"


BluetoothServicesModel::~BluetoothServicesModel() {}

SavedBluetoothServicesModel::~SavedBluetoothServicesModel()
{
    toSettings();
    settings.sync();
}
