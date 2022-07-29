/*
 * Code   : Unit Test Code
 * Date   : 29/07/2022 @ 01:56
 */

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "fpsR305.h"
#include "dir_handler.h"
#include "gpio_driver.h"

/* GPIO Pin Macros */
#define RED_LED_PIN     1
#define GREEN_LED_PIN   24
#define BLUE_LED_PIN    23
#define ONOFF_BUT_PIN   21
#define PLYPS_BUT_PIN   20
#define STOP_BUT_PIN    16

using std::cout; using std::cin;
using std::endl; using std::string;

class sensorTest : public ::testing::Test
{
    protected:
    virtual void SetUp(void)
    {
        std::cout << "Testing R305 Finger Print Scanner Sensor\n";
    }
    virtual void TearDown(void)
    {
    }
    FPS_R305 FPS("/dev/serial0", 57600);
};

TEST_F(R305_FPS_TEST, INIT)
{
    EXPECT_EQ(FPS.init(), true) << "R305 FPS sensor failed to initialize!!\n";
}

class DBTest : public ::testing::Test
{
    protected:
    virtual void SetUp(void)
    {
        std::cout << "Testing DB Entries\n";
    }
    virtual void TearDown(void)
    {
    }
    DIR_HANDLER dhand;
};

TEST_F(USER_DB_TEST, USER_DB_ENTRIES)
{
    bool flag = false;
    dhand.load("../../data/");
    if(dhand.get_count() > 0)
        flag = true;
    EXPECT_EQ(flag, true) << "User database (/data dir.) is empty!!\n";
}

class GPIOTest : public ::testing::Test
{
    protected:
    virtual void SetUp(void)
    {
        std::cout << "Testing GPIO Pins\n";
    }
    virtual void TearDown(void)
    {
    }

    const uint8_t RGB_LED_PINS[3] = {RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN};
    const uint8_t BUTTON_PINS[3] = {ONOFF_BUT_PIN, PLYPS_BUT_PIN, STOP_BUT_PIN};

    GPIO_DRIVER gpio_obj;
};

TEST_F(GPIO_TEST, GPIO_INIT)
{
    EXPECT_EQ(gpio_obj.gpio_init(RGB_LED_PINS[0], GPIO_OUT), GPIO_ERR) << "Failed to initialize GPIO!\n";
    EXPECT_EQ(gpio_obj.gpio_init(RGB_LED_PINS[1], GPIO_OUT), GPIO_ERR) << "Failed to initialize GPIO!\n";
    EXPECT_EQ(gpio_obj.gpio_init(RGB_LED_PINS[2], GPIO_OUT), GPIO_ERR) << "Failed to initialize GPIO!\n";
    EXPECT_EQ(gpio_obj.gpio_init(BUTTON_PINS[0], GPIO_IN), GPIO_ERR) << "Failed to initialize GPIO!\n";
    EXPECT_EQ(gpio_obj.gpio_init(BUTTON_PINS[1], GPIO_IN), GPIO_ERR) << "Failed to initialize GPIO!\n";
    EXPECT_EQ(gpio_obj.gpio_init(BUTTON_PINS[2], GPIO_IN), GPIO_ERR) << "Failed to initialize GPIO!\n";
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
