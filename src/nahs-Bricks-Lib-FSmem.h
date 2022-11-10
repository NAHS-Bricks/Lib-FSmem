#ifndef NAHS_BRICK_LIB_FSMEM_H
#define NAHS_BRICK_LIB_FSMEM_H

#include <Arduino.h>
#include <ArduinoJson.h>

class NahsBricksLibFsMem {
    private:
        StaticJsonDocument<1024> _json;
        StaticJsonDocument<128> _registeredNames;
        bool _autoclean;
    public:
        NahsBricksLibFsMem();
        bool write();
        JsonObject registerData(String name);
        void print();
        void clean();
        void setAutoclean(bool clean = true);
        JsonObject sideInjected(String name);
        void destroy();  // deletes all data in FSmem
    private:
        bool read();
        void init();
};

#if !defined(NO_GLOBAL_INSTANCES)
extern NahsBricksLibFsMem FSmem;
#endif

#endif // NAHS_BRICK_LIB_FSMEM_H
