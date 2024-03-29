/* Copyright 2019 The SMaRC project (https://smarc.se/)
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IMC_HANDLE_H
#define IMC_HANDLE_H

#include "udp_link.hpp"
#include <map>

class IMCHandle {

private:

    rclcpp::Node::SharedPtr node;
    std::string bridge_addr;
    int bridge_port;
    std::string neptus_addr;
    std::string sys_name;
    int imc_id;
	int imc_src;
    
    UDPLink udp_link;
    rclcpp::TimerBase::SharedPtr timer_announce;
    rclcpp::TimerBase::SharedPtr timer_heartbeat;
    // we might need multiple for every message in the future, let's start here
    std::map<uint16_t, std::function<void(const IMC::Message*)>> callbacks;

    double lat;

public:

    IMCHandle(rclcpp::Node::SharedPtr ros_node,
              const std::string& bridge_addr, 
              const int& bridge_port,
              const std::string& neptus_addr,
              const std::string& sys_name, 
              int imc_id, 
              int imc_src);

    ~IMCHandle();

    void announce();

    void publish_heartbeat();

    void udp_subscribe(uint16_t uid, std::function<void(const IMC::Message*)> callback);

    void udp_callback(const IMC::Message* msg);

    template <typename IMC_MSG>
    void write(IMC_MSG& imc_msg)
    {
        udp_link.publish(imc_msg, neptus_addr);
    }

};

#endif // IMC_HANDLE_H
