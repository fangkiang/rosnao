#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alvisiondefinitions.h>

#include <signal.h>
#include <iostream>
#include "rosnao/imager.hpp"

rosnao::ImagePublisher<rosnao::kVGA> *pub_vga = nullptr;
rosnao::ImagePublisher<rosnao::kQVGA> *pub_qvga = nullptr;


volatile std::sig_atomic_t processing_interrupted = false;
extern "C" void interrupt_processing( int ) {
    processing_interrupted = true;
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        std::cerr << "nao_ip, shm_id, res {1=QVGA, 2=VGA}, cam {0=top,1=bottom}" << std::endl;
        return 1;
    }
    processing_interrupted = false;
    std::signal( SIGINT, & interrupt_processing );

    const std::string nao_ip = argv[1];
    const std::string shm_id = argv[2];
    const int res = std::stoi(argv[3]);
    const int cam = std::stoi(argv[4]);

    assert(cam == 0 || cam == 1); // cam must be 0 (top) or 1 (bottom).

    if (res == rosnao::kVGA)
    {
        pub_vga = new rosnao::ImagePublisher<rosnao::kVGA>(nao_ip, shm_id, 30, cam);
        while (!processing_interrupted)
        {
            pub_vga->pub();
        }
        delete pub_vga;
    }
    else if (res == rosnao::kQVGA)
    {
        pub_qvga = new rosnao::ImagePublisher<rosnao::kQVGA>(nao_ip, shm_id, 30, cam);
        while (!processing_interrupted)
        {
            pub_qvga->pub();
        }
        delete pub_qvga;
    }
    else
    {
        assert(false); // res must be 1 (QVGA) or 2 (VGA)
    }

    std::signal( SIGINT, SIG_DFL );
    return 0;
}