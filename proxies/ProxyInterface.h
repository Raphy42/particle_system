//
// Created by Raphaël Dantzer on 13/10/16.
//

#ifndef PARTICLE_SYSTEM_PROXY_INTERFACE_H
#define PARTICLE_SYSTEM_PROXY_INTERFACE_H

namespace Proxy {
    class ProxyInterface {
    public:
        virtual ~ProxyInterface() {};

    private:
        virtual void    PreInit() = 0;
        virtual void    Init() = 0;
        virtual void    PostInit() = 0;
    };
}

#endif //PARTICLE_SYSTEM_PROXY_INTERFACE_H
