//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __BULLY_TCX_H
#define __BULLY_TCX_H

#include <omnetpp.h>
#include "nMsg_m.h"


using namespace omnetpp;

namespace bully {

/**
 * Implements the Txc simple module. See the NED file for more information.
 */
class Node : public cSimpleModule
{ private:

    nMsg *CALL_HEART_BEAT_MSG;
    nMsg *HEARTBEAT_TIME_OUT_MSG;

    nMsg *TIME_OUT_MSG;
    nMsg *SELF_MSG;
    nMsg *RETURN_OF_FAIL_MSG;

    nMsg *ELECTION_MSG;
    nMsg *LEADER_MSG;
    nMsg *REPLY_MSG;
    nMsg *INITIAL_MSG;
    nMsg *HANDLER_MSG;
    nMsg *HEARTBEAT_MSG;

    int id;
    int leaderId;
    int state;
    bool failed;
    bool nodeFault;
    int numbersOfNodes;
    int chanceOfError;

  protected:
    virtual void initialize();
    virtual void changeColorAndIcon(char* color, char* icon);
    virtual void bullyAlgorithm(cMessage *nmsg);
    virtual void handleMessage(cMessage *msg);
    virtual void heartBeat();
    virtual nMsg* createMsg(const char* msg,int type,int source,int destination,int color);
    virtual void sendElectionMsg();
    virtual void sendLeaderToAll();
};

}; // namespace

#endif
