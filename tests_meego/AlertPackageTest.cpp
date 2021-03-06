/*
* This file is part of buteo-syncml package
*
* Copyright (C) 2010 Nokia Corporation. All rights reserved.
*
* Contact: Sateesh Kavuri <sateesh.kavuri@nokia.com>
*
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* Neither the name of Nokia Corporation nor the names of its contributors may 
* be used to endorse or promote products derived from this software without 
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
* 
*/

#include "AlertPackageTest.h"
#include "AlertPackage.h"
#include "SyncMLMessage.h"
#include "TestLoader.h"
#include "Mock.h"
#include "QtEncoder.h"

using namespace DataSync;

void AlertPackageTest::testConstruction()
{

    QString data = QString::number( TWO_WAY_SYNC );
    QString source( "src" );
    QString target( "dst" );
    QString type( "type" );

    // One test block for each constructor type.
    {
        AlertPackage pkg(data.toInt());
        CommandParams result_params = pkg.iParams;
        QVERIFY( result_params.commandType == CommandParams::COMMAND_ALERT );
        QCOMPARE(result_params.data, data);
    }
    {
        AlertPackage pkg(data.toInt(), source, target);
        CommandParams result_params = pkg.iParams;
        QVERIFY( result_params.commandType == CommandParams::COMMAND_ALERT );
        QCOMPARE(result_params.data, data);
        QCOMPARE(result_params.items.count(), 1 );
        QCOMPARE(result_params.items.first().source, source);
        QCOMPARE(result_params.items.first().target, target);
    }

    {
        AlertPackage pkg( data.toInt(), source, target, "", "" );
        CommandParams result_params = pkg.iParams;
        QVERIFY( result_params.commandType == CommandParams::COMMAND_ALERT );
        QCOMPARE(result_params.data, data);
        QCOMPARE(result_params.items.count(), 1 );
        QCOMPARE(result_params.items.first().source, source);
        QCOMPARE(result_params.items.first().target, target);
        QVERIFY(result_params.items.first().meta.anchor.last == "0");
    }

    {
        AlertPackage pkg( source, type, data.toInt() );
        CommandParams result_params = pkg.iParams;
        QVERIFY( result_params.commandType == CommandParams::COMMAND_ALERT );
        QCOMPARE(result_params.data, data);
        QCOMPARE(result_params.items.count(), 1 );
        QCOMPARE(result_params.items.first().source, source);
        QCOMPARE(result_params.items.first().meta.type, type);
    }

}

void AlertPackageTest::testWrite()
{
    AlertPackage pkg(SLOW_SYNC, "foo", "bar");
    SyncMLMessage msg(HeaderParams(), SYNCML_1_2);

    QtEncoder encoder;
    QByteArray initial_xml;
    QVERIFY( encoder.encodeToXML( msg, initial_xml, true ) );


    const int SIZE_TRESHOLD = 10000;
    int remaining = SIZE_TRESHOLD;
    QCOMPARE(pkg.write(msg, remaining, false, SYNCML_1_2), true);
    QVERIFY(remaining < SIZE_TRESHOLD);
    QByteArray result_xml;
    QVERIFY( encoder.encodeToXML( msg, result_xml, true ) );
    QVERIFY(result_xml.size() > initial_xml.size());
    QVERIFY(result_xml.indexOf(SYNCML_ELEMENT_ALERT) != -1);
}

TESTLOADER_ADD_TEST(AlertPackageTest);
