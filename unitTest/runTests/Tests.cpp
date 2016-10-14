//
// Created by peer23peer on 7/18/16.
//

#include "gtest/gtest.h"

#include "../../ohCaptain/include/Controllers/BeagleboneBlack.h"
#include "../../ohCaptain/include/Vessels/Meetcatamaran.h"
#include "../../ohCaptain/include/Communication/LoRa_RN2483.h"
#include "../../ohCaptain/include/Sensors/Razor.h"

#include <boost/filesystem.hpp>

TEST(Controller, modLoaded) {
    class wrap : public oCpt::protocol::userspace {
    public:
        bool wrapper(std::string str) {
            return modLoaded(str);
        }
    } w;
    ASSERT_TRUE(w.wrapper("ti_am335x_adc"));
    ASSERT_FALSE(w.wrapper("jgsahjgsaj"));
}

TEST(Controller, dtboLoaded) {
    class wrap : public oCpt::protocol::userspace {
    public:
        bool wrapper(std::string str) {
            return dtboLoaded(str);
        }
    } w;
    ASSERT_TRUE(w.wrapper("MTI-CATAMARAN"));
    ASSERT_FALSE(w.wrapper("jgsahjgsaj"));
}

TEST(Controller, fileExist) {
    class wrap : public oCpt::protocol::userspace {
    public:
        bool wrapper(std::string str) {
            return fileExist(str);
        }
    } w;
    ASSERT_TRUE(w.wrapper("runTests"));
    ASSERT_FALSE(w.wrapper("jgsahjgsaj"));
}

TEST(Controller, exportedGpios) {
    boost::filesystem::path p("/sys/class/gpio/");
    size_t sum = 0;
    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr) {
        sum++;
    }
    std::vector<oCpt::protocol::gpio::ptr> v_gpio = oCpt::protocol::gpio::exportedGpios();
    ASSERT_EQ(v_gpio.size(), sum - 7);
}

TEST(Controller, gpio_input) {
    using namespace oCpt::protocol;
    oCpt::protocol::gpio p(60, gpio::Direction::INPUT);
    ASSERT_EQ(p.getValue(), gpio::Value::LOW);
}

TEST(Controller, gpio_output) {
    using namespace oCpt::protocol;
    oCpt::protocol::gpio p(60, gpio::Direction::OUTPUT);
    ASSERT_EQ(p.getValue(), gpio::Value::LOW);
    p.setValue(gpio::Value::HIGH);
    ASSERT_EQ(p.getValue(), gpio::Value::HIGH);
}

TEST(Controller, gpio_toggle) { //TODO implement assert
    using namespace oCpt::protocol;
    gpio p(60, gpio::Direction::OUTPUT);

    for (int i = 0; i < 100; i++) {
        p.toggle();
    }
}

TEST(Controller, gpio_poll) { //TODO implement assert
    using namespace oCpt::protocol;
    gpio p(60, gpio::Direction::INPUT, gpio::Value::LOW, gpio::Edge::BOTH);
    p.signalChanged.connect([]{
        std::cout << "input recieved!" << std::endl;
    });
    //p.waitForEdgeAsync();
}


TEST(Sensor, Razor) {
    using namespace oCpt::components::sensors;
    using namespace oCpt::components::controller;
    using namespace oCpt;
    using namespace oCpt::vessels;

    World::ptr w(new World());
    BBB::ptr bbb(new BBB(w));

    Razor::ptr r(new Razor(bbb, w, "Razor", "/dev/ttyS4", 57600));
    r->init();
    Razor::ReturnValue_t retVal = CAST(r->getState().Value);
    std::cout << std::to_string(retVal.mag[0]) << std::endl;

}
//TEST(Meetcatamaran, SerielTest) {
//    oCpt::protocol::Serial serial("/dev/ttyACM0", 57600);
//    serial.open();
//    serial.msgRecievedSig.connect([&]{
//        std::cout << serial.readFiFoMsg() << std::endl; });
//    serial.start();
//    serial.write("test");
//    for(;;) {}
//}

//TEST(Meetcatamaran, ADC_read) {
//    //oCpt::vessels::Meetcatamaran::ptr mc(new oCpt::vessels::Meetcatamaran());
//    //mc->initialize();
//    //mc->run();
//
//    EXPECT_EQ(1,1);
//}
//

/*
TEST(Meetcatamaran, LoRaTest) {
    oCpt::components::comm::LoRa_RN2483 loRaRn2483("loRaRn2483", "/dev/ttyACM0");
    loRaRn2483.initialize();
    loRaRn2483.msgRecievedSig.connect([&]{
        std::cout << loRaRn2483.readFiFoMsg()->Payload << std::endl;
    });
    loRaRn2483.run();
    for(;;);
    loRaRn2483.stop();
}

*/
