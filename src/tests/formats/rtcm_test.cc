/*!
 * \file rtcm_test.cc
 * \brief  This file implements unit tests for the Rtcm class.
 * \author Carles Fernandez-Prades, 2015. cfernandez(at)cttc.es
 *
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2015  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#include <memory>
#include "rtcm.h"

TEST(Rtcm_Test, Hex_to_bin)
{
    auto rtcm = std::make_shared<Rtcm>();

    std::string test1 = "2A";
    std::string test1_bin = rtcm->hex_to_bin(test1);
    EXPECT_EQ(0, test1_bin.compare("00101010"));

    std::string test2 = "FF";
    std::string test2_bin = rtcm->hex_to_bin(test2);
    EXPECT_EQ(0, test2_bin.compare("11111111"));

    std::string test3 = "ff";
    std::string test3_bin = rtcm->hex_to_bin(test3);
    EXPECT_EQ(0, test3_bin.compare("11111111"));

    std::string test4 = "100";
    std::string test4_bin = rtcm->hex_to_bin(test4);
    EXPECT_EQ(0, test4_bin.compare("000100000000"));

    std::string test5 = "1101";
    std::string test5_bin = rtcm->hex_to_bin(test5);
    EXPECT_EQ(0, test5_bin.compare("0001000100000001"));

    std::string test6 = "3";
    std::string test6_bin = rtcm->hex_to_bin(test6);
    EXPECT_EQ(0, test6_bin.compare("0011"));
}


TEST(Rtcm_Test, Bin_to_hex)
{
    auto rtcm = std::make_shared<Rtcm>();

    std::string test1 = "00101010";
    std::string test1_hex = rtcm->bin_to_hex(test1);
    EXPECT_EQ(0, test1_hex.compare("2A"));

    std::string test2 = "11111111";
    std::string test2_hex = rtcm->bin_to_hex(test2);
    EXPECT_EQ(0, test2_hex.compare("FF"));

    std::string test4 = "000100000000";
    std::string test4_hex = rtcm->bin_to_hex(test4);
    EXPECT_EQ(0, test4_hex.compare("100"));

    std::string test5 = "0001000100000001";
    std::string test5_hex = rtcm->bin_to_hex(test5);
    EXPECT_EQ(0, test5_hex.compare("1101"));

    std::string test6 = "0011";
    std::string test6_hex = rtcm->bin_to_hex(test6);
    EXPECT_EQ(0, test6_hex.compare("3"));

    std::string test7 = "11";
    std::string test7_hex = rtcm->bin_to_hex(test7);
    EXPECT_EQ(0, test7_hex.compare("3"));

    std::string test8 = "1000100000001";
    std::string test8_hex = rtcm->bin_to_hex(test8);
    EXPECT_EQ(0, test8_hex.compare("1101"));
}



TEST(Rtcm_Test, Hex_to_int)
{
    auto rtcm = std::make_shared<Rtcm>();

    std::string test1 = "2A";
    long int test1_int = rtcm->hex_to_int(test1);
    long int expected1 = 42;
    EXPECT_EQ(expected1, test1_int);
}


TEST(Rtcm_Test, Hex_to_uint)
{
    auto rtcm = std::make_shared<Rtcm>();
    long unsigned int expected1 = 42;
    EXPECT_EQ(expected1,  rtcm->hex_to_uint(rtcm->bin_to_hex("00101010")));
}


TEST(Rtcm_Test, Bin_to_double)
{
    auto rtcm = std::make_shared<Rtcm>();

    std::bitset<4> test1(5);
    long int test1_int = static_cast<long int>(rtcm->bin_to_double(test1.to_string()));
    long int expected1 = 5;
    EXPECT_EQ(expected1, test1_int);

    std::bitset<4> test2(-5);
    EXPECT_DOUBLE_EQ(-5, rtcm->bin_to_double(test2.to_string()));

    std::bitset<65> test3(-5);
    EXPECT_DOUBLE_EQ(0, rtcm->bin_to_double(test3.to_string()));
}


TEST(Rtcm_Test, Bin_to_uint)
{
    auto rtcm = std::make_shared<Rtcm>();
    long unsigned int expected1 = 42;
    EXPECT_EQ(expected1, rtcm->bin_to_uint("00101010"));
    long unsigned int expected2 = 214;
    EXPECT_EQ(expected2, rtcm->bin_to_uint("11010110"));
}


TEST(Rtcm_Test, Bin_to_int)
{
    auto rtcm = std::make_shared<Rtcm>();
    long unsigned int expected1 = 42;
    EXPECT_EQ(expected1, rtcm->bin_to_int("00101010"));
    long unsigned int expected2 = -42;
    EXPECT_EQ(expected2, rtcm->bin_to_int("11010110"));
}


TEST(Rtcm_Test, Check_CRC)
{
    auto rtcm = std::make_shared<Rtcm>();
    EXPECT_EQ(true, rtcm->check_CRC("D300133ED7D30202980EDEEF34B4BD62AC0941986F33360B98"));
    EXPECT_EQ(false, rtcm->check_CRC("D300133ED7D30202980EDEEF34B4BD62AC0941986F33360B99"));

    EXPECT_EQ(true, rtcm->check_CRC(rtcm->print_MT1005_test()));
    EXPECT_EQ(true, rtcm->check_CRC(rtcm->print_MT1005_test()));  // Run twice to check that CRC has no memory
}


TEST(Rtcm_Test, Test_MT1005)
{
    auto rtcm = std::make_shared<Rtcm>();
    std::string reference_msg = rtcm->print_MT1005_test();
    std::string reference_msg2 = rtcm->print_MT1005(2003, 1114104.5999, -4850729.7108, 3975521.4643, true, false, false, false, false, 0);
    EXPECT_EQ(0, reference_msg.compare(reference_msg2));

    unsigned int ref_id;
    double ecef_x;
    double ecef_y;
    double ecef_z;
    bool gps;
    bool glonass;
    bool galileo;

    rtcm->read_MT1005(reference_msg, ref_id, ecef_x, ecef_y, ecef_z, gps, glonass, galileo);

    EXPECT_EQ(true, gps);
    EXPECT_EQ(false, glonass);
    EXPECT_EQ(false, galileo);

    EXPECT_EQ(2003, ref_id);
    EXPECT_DOUBLE_EQ(1114104.5999, ecef_x);
    EXPECT_DOUBLE_EQ(-4850729.7108, ecef_y);
    EXPECT_DOUBLE_EQ(3975521.4643, ecef_z);

    rtcm->read_MT1005("D300133ED7D30202980EDEEF34B4BD62AC0941986F33360B98", ref_id, ecef_x, ecef_y, ecef_z, gps, glonass, galileo);

    EXPECT_EQ(true, gps);
    EXPECT_EQ(false, glonass);
    EXPECT_EQ(false, galileo);

    EXPECT_EQ(2003, ref_id);
    EXPECT_DOUBLE_EQ(1114104.5999, ecef_x);
    EXPECT_DOUBLE_EQ(-4850729.7108, ecef_y);
    EXPECT_DOUBLE_EQ(3975521.4643, ecef_z);
}



TEST(Rtcm_Test, Test_MT1019)
{
    auto rtcm = std::make_shared<Rtcm>();

    Gps_Ephemeris gps_eph = Gps_Ephemeris();
    Gps_Ephemeris gps_eph_read = Gps_Ephemeris();

    gps_eph.i_satellite_PRN = 3;
    gps_eph.d_IODC = 4;
    gps_eph.d_e_eccentricity = 2.0 * E_LSB;
    gps_eph.b_fit_interval_flag = true;
    std::string tx_msg = rtcm->print_MT1019(gps_eph);

    EXPECT_EQ(0, rtcm->read_MT1019(tx_msg, gps_eph_read));
    EXPECT_EQ(3, gps_eph_read.i_satellite_PRN);
    EXPECT_DOUBLE_EQ(4, gps_eph_read.d_IODC);
    EXPECT_DOUBLE_EQ( 2.0 * E_LSB, gps_eph_read.d_e_eccentricity);
    EXPECT_EQ(true, gps_eph_read.b_fit_interval_flag);
    EXPECT_EQ(1, rtcm->read_MT1019("FFFFFFFFFFF", gps_eph_read));
}



TEST(Rtcm_Test, Test_MT1045)
{
    auto rtcm = std::make_shared<Rtcm>();

    Galileo_Ephemeris gal_eph = Galileo_Ephemeris();
    Galileo_Ephemeris gal_eph_read = Galileo_Ephemeris();

    gal_eph.i_satellite_PRN = 5;
    gal_eph.OMEGA_dot_3 = 53.0 * OMEGA_dot_3_LSB;
    gal_eph.E5a_DVS = true;

    std::string tx_msg = rtcm->print_MT1045(gal_eph);

    EXPECT_EQ(0, rtcm->read_MT1045(tx_msg, gal_eph_read));
    EXPECT_EQ(true, gal_eph_read.E5a_DVS);
    EXPECT_DOUBLE_EQ( 53.0 * OMEGA_dot_3_LSB, gal_eph_read.OMEGA_dot_3);
    EXPECT_EQ(5, gal_eph_read.i_satellite_PRN);
    EXPECT_EQ(1, rtcm->read_MT1045("FFFFFFFFFFF", gal_eph_read));
}


TEST(Rtcm_Test, MT1001)
{
    auto rtcm = std::make_shared<Rtcm>();
    Gps_Ephemeris gps_eph = Gps_Ephemeris();
    Gnss_Synchro gnss_synchro;
    gnss_synchro.PRN = 2;
    std::string sys = "G";

    std::string sig = "1C";
    gnss_synchro.System = *sys.c_str();
    std::memcpy((void*)gnss_synchro.Signal, sig.c_str(), 3);
    gnss_synchro.Pseudorange_m = 20000000.0;
    double obs_time = 25.0;
    std::map<int, Gnss_Synchro> pseudoranges;
    pseudoranges.insert(std::pair<int, Gnss_Synchro>(1, gnss_synchro));

    std::string MT1001 = rtcm->print_MT1001(gps_eph, obs_time, pseudoranges);
    EXPECT_EQ(true, rtcm->check_CRC(MT1001));
}


TEST(Rtcm_Test, MSM1)
{
    auto rtcm = std::make_shared<Rtcm>();
    Gps_Ephemeris gps_eph = Gps_Ephemeris();
    Galileo_Ephemeris gal_eph = Galileo_Ephemeris();
    std::map<int, Gnss_Synchro> pseudoranges;

    Gnss_Synchro gnss_synchro;
    Gnss_Synchro gnss_synchro2;
    Gnss_Synchro gnss_synchro3;

    gnss_synchro.PRN = 2;
    gnss_synchro2.PRN = 4;
    gnss_synchro3.PRN = 32;

    std::string sys = "G";

    std::string sig = "1C";
    std::string sig2 = "2S";

    gnss_synchro.System = *sys.c_str();
    gnss_synchro2.System = *sys.c_str();
    gnss_synchro3.System = *sys.c_str();

    std::memcpy((void*)gnss_synchro.Signal, sig.c_str(), 3);
    std::memcpy((void*)gnss_synchro2.Signal, sig.c_str(), 3);
    std::memcpy((void*)gnss_synchro3.Signal, sig2.c_str(), 3);

    gnss_synchro.Pseudorange_m = 20000000.0;
    gnss_synchro2.Pseudorange_m = 20000010.0;
    gnss_synchro3.Pseudorange_m = 20000020.0;

    pseudoranges.insert(std::pair<int, Gnss_Synchro>(1, gnss_synchro));
    pseudoranges.insert(std::pair<int, Gnss_Synchro>(2, gnss_synchro2));
    pseudoranges.insert(std::pair<int, Gnss_Synchro>(3, gnss_synchro3));

    unsigned int ref_id = 1234;
    unsigned int clock_steering_indicator = 0;
    unsigned int external_clock_indicator = 0;
    int smooth_int = 0;
    bool sync_flag = false;
    bool divergence_free = false;
    bool more_messages = false;
    double obs_time = 25.0;

    gps_eph.i_satellite_PRN = gnss_synchro.PRN;

    std::string MSM1 = rtcm->print_MSM_1(gps_eph,
            gal_eph,
            obs_time,
            pseudoranges,
            ref_id,
            clock_steering_indicator,
            external_clock_indicator,
            smooth_int,
            sync_flag,
            divergence_free,
            more_messages);

    EXPECT_EQ(true, rtcm->check_CRC(MSM1));
}

