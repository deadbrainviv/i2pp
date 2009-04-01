/*  this file is part of i2pp
    Copyright (C)2009 Gilbert Jeiziner

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "pc.h"
#include "ntpmessage.h"

#include <math.h>
#include <cstdlib>

using namespace i2pp::core;

const quint32 NtpMessage::_secondsTo1970 = 2208988800U;

bool bRandInit = false;

NtpMessage::NtpMessage()
{
    _leapIndicator = 0;
    _version = 3;
    _mode = 3;
    _stratum = 0;
    _pollInterval = 0;
    _precision = 0;
    _rootDelay = 0.0;
    _rootDispersion = 0.0;
    _referenceIdentifier = QByteArray((int)4,(byte)0);
    nowUTC(_transmitTime);
}

NtpMessage::NtpMessage(const QByteArray& ba)
{
    ///@todo parse the byte array
    ba;
}

//static
void NtpMessage::nowUTC(ntpTime& time)
{
    QDateTime current = QDateTime::currentDateTime();
    //toTime_t also converts to UTC
    time._seconds = _secondsTo1970 + current.toTime_t();
    time._fractional = current.time().msec();
}

QByteArray NtpMessage::toByteArray()
{
    QByteArray ba(int(48),(byte)0);
    ba[0] = (byte) (_leapIndicator << 6 | _version << 3 | _mode);
    ba[1] = (byte) _stratum;
    ba[2] = (byte) _pollInterval;
    ba[3] = (byte) _precision;

    // root delay is a signed 16.16-bit FP
    qint16 l = (qint16) (_rootDelay * 65536.0);
    ba[4] = (byte) ((l >> 24) & 0xFF);
    ba[5] = (byte) ((l >> 16) & 0xFF);
    ba[6] = (byte) ((l >> 8) & 0xFF);
    ba[7] = (byte) (l & 0xFF);

    // root dispersion is an unsigned 16.16-bit FP
    quint16 ul = (quint16) (_rootDispersion * 65536.0);
    ba[8] = (byte) ((ul >> 24) & 0xFF);
    ba[9] = (byte) ((ul >> 16) & 0xFF);
    ba[10] = (byte) ((ul >> 8) & 0xFF);
    ba[11] = (byte) (ul & 0xFF);

    ba[12] = _referenceIdentifier[0];
    ba[13] = _referenceIdentifier[1];
    ba[14] = _referenceIdentifier[2];
    ba[15] = _referenceIdentifier[3];

    encodeTimestamp(ba, 16, _referenceTime);
    encodeTimestamp(ba, 24, _originateTime);
    encodeTimestamp(ba, 32, _recieveTime);
    encodeTimestamp(ba, 40, _transmitTime);
    return ba;
}

void NtpMessage::encodeTimestamp(QByteArray& ba, int startIndex, ntpTime& timeStamp)
{
    ba[startIndex] = (byte) ((timeStamp._seconds >> 24) & 0xFF);
    ba[startIndex+1] = (byte) ((timeStamp._seconds >> 16) & 0xFF);
    ba[startIndex+2] = (byte) ((timeStamp._seconds >> 8) & 0xFF);
    ba[startIndex+3] = (byte) (timeStamp._seconds & 0xFF);
    ba[startIndex+4] = (byte) ((timeStamp._fractional >> 24) & 0xFF);
    ba[startIndex+5] = (byte) ((timeStamp._fractional >> 16) & 0xFF);
    ba[startIndex+6] = (byte) ((timeStamp._fractional >> 8) & 0xFF);
    // From RFC 2030: It is advisable to fill the non-significant
    // low order bits of the timestamp with a random, unbiased
    // bitstring, both to avoid systematic roundoff errors and as
    // a means of loop detection and replay detection.
    // the below should be sufficient for our needs
    if (!bRandInit)
    {
        bRandInit = true;
        srand(timeStamp._seconds);
    }
    ba[startIndex+7] = (byte) (rand() % 255);
}
