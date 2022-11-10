#include <nahs-Bricks-Lib-FSmem.h>
#include <LittleFS.h>

NahsBricksLibFsMem::NahsBricksLibFsMem() {
    _autoclean = true;
    if (!read()) init();
}

bool NahsBricksLibFsMem::write() {
    if (_autoclean) clean();
    _json["_s"] = 1;  // Setting current schema version
    bool result = true;
    File cfgFile = LittleFS.open("/FSmem.json", "w");
    if (cfgFile) {
        serializeJson(_json, cfgFile);
        cfgFile.close();
    } else result = false;
    return result;
}

JsonObject NahsBricksLibFsMem::registerData(String name) {
    bool found = false;
    for (JsonVariant value : _registeredNames.as<JsonArray>()) {
        if (value.as<String>() == name) {
            found = true;
            break;
        }
    }
    if (!found) _registeredNames.add(name);

    if (!_json.containsKey(name)) {
        _json.createNestedObject(name);
    }
    return _json[name].as<JsonObject>();
}

void NahsBricksLibFsMem::print() {
    Serial.println("Registered Names:");
    serializeJson(_registeredNames, Serial);
    Serial.println();

    Serial.println("JSON Document:");
    serializeJsonPretty(_json, Serial);
    Serial.println();
    Serial.println();
}

void NahsBricksLibFsMem::clean() {
    for (JsonPair keyVal : _json.as<JsonObject>()) {  // iterate over all keys in _json
        String key = String(keyVal.key().c_str());
        if (key[0] == '_') continue;  // skip the ones starting with _ (these should not be touched as they are schema related entries)

        bool found = false;
        for (JsonVariant value : _registeredNames.as<JsonArray>()) {  // Search for the key in _registeredNames
            if (value.as<String>() == key) {
                found = true;
                break;
            }
        }

        if (!found) {  // If the key is not in _registeredNames delete it from _json
            _json.remove(key);
        }
    }
}

void NahsBricksLibFsMem::setAutoclean(bool clean) {
    _autoclean = clean;
}

JsonObject NahsBricksLibFsMem::sideInjected(String name) {
    if (!_json.containsKey(name)) {
        _json.createNestedObject(name);
    }
    return _json[name].as<JsonObject>();
}

void NahsBricksLibFsMem::destroy() {
    _registeredNames.clear();
    clean();
}

bool NahsBricksLibFsMem::read() {
    LittleFS.begin();
    bool result = true;
    File cfgFile = LittleFS.open("/FSmem.json", "r");
    if (cfgFile) {
        if (cfgFile.size() <= 1024) {
            DeserializationError error = deserializeJson(_json, cfgFile);
            if (error) result = false;
        } else result = false;
        cfgFile.close();
    } else result = false;
    return result;
}

void NahsBricksLibFsMem::init() {
    _json["_s"] = 1;  // Setting schema version
}


//------------------------------------------
// globally predefined variable
#if !defined(NO_GLOBAL_INSTANCES)
NahsBricksLibFsMem FSmem;
#endif
