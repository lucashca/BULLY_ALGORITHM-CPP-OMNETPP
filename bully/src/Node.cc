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

#include "Node.h"

#include "nMsg_m.h"

namespace bully {

    Define_Module(Node);

    const char* HEART_BEAT = "Heart Beat";
    const char* ELECTION = "Election";
    const char* INITIAL = "Initial Message";
    const char* HANDLER = "Handler Message";
    const char* CALL_HEART_BEAT = "Calling Heart Beat";
    const char* TIME_OUT = "Time out";
    const char* RETURN_OF_FAIL = "Node come back";
    const char* LEADER = "I am leader";
    const char* REPLY = "Reply";
    const char* SELF = "Self call";

    //Number of I/O gates
    int numbersOfGates;

    //Destination to send a message
    int destination;

    //Count of received messages to generate a new message
    int cont_to_generate_msg;
    enum STATES {
        IDLE = 1,
        WAIT_FOR_REPLY,
        WAIT_FOR_LEADER,
    };


    //Types of messages to check
    enum MESSAGES {
        HEART_BEAT_ENUM = 1,
        ELECTION_ENUM,
        INITIAL_ENUM,
        HANDLER_ENUM,
        REPLY_ENUM,
        CALL_HEART_BEAT_ENUM,
        TIME_OUT_ENUM,
        RETURN_OF_FAIL_ENUM,
        SELF_ENUM,
        LEADER_ENUM,
        HEARTBEAT_TIME_OUT_ENUM
    };

    // Colors of messages
    enum COLORS {
        RED = 0 ,
        GREEN ,
        BLUE,
        WHITE,
        YELLOW,
        CYAN,
        MAGENTA,
        BLACK
    };


    void Node::initialize()
    {

        cDisplayString& displayString = getDisplayString();
        displayString.setTagArg("i", 0, "device/pc3_vl");

        cont_to_generate_msg = 0;

        //SETTING ATRIBUTS
        id = getIndex();
        leaderId = 0;
        state = IDLE;
        failed = false;
        nodeFault = false;

        numbersOfNodes = par("numberOfNode").intValue();
        numbersOfGates = numbersOfNodes -1;
        chanceOfError = par("chanceOfError").intValue();

        //CREATING DEFAULT MESSAGES
        CALL_HEART_BEAT_MSG = new nMsg(CALL_HEART_BEAT);

        //Schedule Messages
        TIME_OUT_MSG = new nMsg(TIME_OUT);
        TIME_OUT_MSG->setType(TIME_OUT_ENUM);
        HEARTBEAT_TIME_OUT_MSG = new nMsg(TIME_OUT);
        HEARTBEAT_TIME_OUT_MSG->setType(HEARTBEAT_TIME_OUT_ENUM);
        RETURN_OF_FAIL_MSG = new nMsg(RETURN_OF_FAIL);
        SELF_MSG = new nMsg(SELF);

        //Messages to send
        ELECTION_MSG = new nMsg(ELECTION);
        LEADER_MSG = new nMsg(LEADER);
        REPLY_MSG = new nMsg(RETURN_OF_FAIL);
        INITIAL_MSG = new nMsg(INITIAL);
        HANDLER_MSG = new nMsg(HANDLER);
        HEARTBEAT_MSG = new nMsg(HANDLER);


        if (par("sendInitialMessage").boolValue())
        {
            destination = rand() % numbersOfGates;
            INITIAL_MSG = createMsg(INITIAL, INITIAL_ENUM, id, destination,BLUE);
            //EV << "Node "<<id<<" is sending Msg:"<< msg <<" to destination "<< destination << " \n";
            send(INITIAL_MSG, "out",destination);

        }

        if (leaderId == id){
            heartBeat();
        }
    }


    void Node::sendElectionMsg()
    {
        int i;
        for(i = id; i<numbersOfGates; i++){
            ELECTION_MSG = createMsg(ELECTION, ELECTION_ENUM, id, i,RED);
            send(ELECTION_MSG, "out",i);
        }

    }
    void Node::sendLeaderToAll()
    {
        int i;
        for(i = 0; i < numbersOfGates; i++){
                EV << "I "<<i<<"  \n";
                LEADER_MSG = createMsg(LEADER, LEADER_ENUM, id, i,GREEN);
                send(LEADER_MSG, "out",i);
        }
    }

    void Node::bullyAlgorithm(cMessage *nmsg){
   
        nMsg *msg = check_and_cast<nMsg *>(nmsg);
        int type = 0;
        int s = msg->getSource();
        int d = msg->getDestination();
        const char* nome = msg->getName();
        type = msg->getType();

	
        //Step 1 - Failure has detected failed = true

        //Step 2 - Send election message for all nodes , change state for WAIT_FOR_REPLY and failed to false

        if(failed){
           changeColorAndIcon((char*)"violet",(char*)"device/pc3_vl");
           sendElectionMsg();
           state = WAIT_FOR_REPLY;
           cancelEvent(TIME_OUT_MSG);
           scheduleAt(simTime()+0.3, TIME_OUT_MSG);
           failed = false;
        }

        // Step 3 - State = IDLE and m = ELECTION_MSG send reply to sender; filed = true;

        if(state == IDLE and type == ELECTION_ENUM){

            changeColorAndIcon((char*)"gold",(char*)"device/pc3_vl");
            failed = true;
            REPLY_MSG = createMsg(REPLY, REPLY_ENUM, id, s,CYAN);
            send(REPLY_MSG, "out",s);
            cancelEvent(SELF_MSG);
            scheduleAt(simTime(), SELF_MSG);
            EV << "state "<<state<<" failed:"<< failed << "\n";
        }


        // Step 3.1 - State != IDLE and m = ELECTION_MSG send reply to sender; filed = true;
        // Correctness of Bully Algorithm

        if(state != IDLE and type == ELECTION_ENUM){
            changeColorAndIcon((char*)"gold",(char*)"device/pc3_vl");
            REPLY_MSG = createMsg(REPLY, REPLY_ENUM, id, s,CYAN);
            send(REPLY_MSG, "out",s);

        }
        // Step 4 - State = WAIT_FOR_REPLY and MSG = REPLY_MSG change state to WAIT_FOR_LEADER
        if(state == WAIT_FOR_REPLY and type == REPLY_ENUM){
            changeColorAndIcon((char*)"white",(char*)"device/pc3_vl");
            state = WAIT_FOR_LEADER;
            cancelEvent(TIME_OUT_MSG);
            scheduleAt(simTime()+1, TIME_OUT_MSG);
        }
        // Step 5 - State = WAIT_FOR_REPLY and MSG = TIME_OUT_MSG update leader id and
        // change state to IDLE and send leader to all
        if(state == WAIT_FOR_REPLY and msg == TIME_OUT_MSG){
            changeColorAndIcon((char*)"red",(char*)"device/pc3_vl");
            leaderId = id;
            state = IDLE;
            failed = false;
            sendLeaderToAll();
            scheduleAt(simTime()+0.1, CALL_HEART_BEAT_MSG);
        }
        // Step 6 - MSG = LEADER_MSG  update leader with sender id change state to IDLE
        if(type == LEADER_ENUM){
            cancelEvent(TIME_OUT_MSG);
            leaderId = s;
            state = IDLE;
            cancelEvent(HEARTBEAT_TIME_OUT_MSG);
            scheduleAt(simTime()+1.0, HEARTBEAT_TIME_OUT_MSG);

        }
        //Step 7 - state = WAIT_FOR_LEADER and msg = TIME_OUT_MSG change failed to true and state to IDLE
        if(state == WAIT_FOR_LEADER and msg == TIME_OUT_MSG){
            state = IDLE;
            failed = true;
        }
    }
    void Node::changeColorAndIcon(char* color, char* icon){
        cDisplayString& displayString = getDisplayString();
        displayString.setTagArg("i", 0, icon);
        displayString.setTagArg("i", 1, color);

    }
    void Node::handleMessage(cMessage *nmsg)
    {

        nMsg *msg = check_and_cast<nMsg *>(nmsg);
        int type = 0;
        int s = msg->getSource();
        int d = msg->getDestination();
        const char* nome = msg->getName();
        type = msg->getType();

        if(nodeFault){
            msg = createMsg(HANDLER, 0, id, id,BLUE);
            type = 0;
            changeColorAndIcon((char*)"red",(char*)"device/pc3_vl");

        }else{

            //Leader algorithm

            bullyAlgorithm(nmsg);


            if(failed == false and state == IDLE){
                changeColorAndIcon((char*)"green",(char*)"device/pc3_vl");


                if(type == HEART_BEAT_ENUM){
                    leaderId = s;
                    cancelEvent(HEARTBEAT_TIME_OUT_MSG);
                    scheduleAt(simTime()+1.0, HEARTBEAT_TIME_OUT_MSG);

                    changeColorAndIcon((char*)"pink",(char*)"device/pc3_vl");
                }

                if(type == HANDLER_ENUM or type ==INITIAL_ENUM ){
                    destination = rand()%numbersOfGates;
                    HANDLER_MSG = createMsg(HANDLER, HANDLER_ENUM, id, destination,BLUE);
                    send(HANDLER_MSG, "out",destination);

                }

                if(msg == HEARTBEAT_TIME_OUT_MSG){

                    changeColorAndIcon((char*)"black",(char*)"device/pc3_vl");

                    failed = true;
                    scheduleAt(simTime(), SELF_MSG);

                }
                if(msg == CALL_HEART_BEAT_MSG){
                    int sortError = rand() % 100 ;
                    if(sortError < chanceOfError){

                        changeColorAndIcon((char*)"red",(char*)"device/pc3_vl");

                        nodeFault = true;
                    }else{
                        heartBeat();

                    }

                }
                if (type == RETURN_OF_FAIL_ENUM){

                    nMsg *m = new nMsg();
                    destination = rand() % numbersOfGates ;
                    HANDLER_MSG = createMsg(HANDLER, HANDLER_ENUM, id, destination,BLUE);
                    send(HANDLER_MSG, "out",destination);
                }
            }
        }
    }

    nMsg* Node::createMsg(const char* msg,int type,int source,int destination,int color){
        nMsg *nmsg = new nMsg();
        nmsg->setName(msg);
        nmsg->setSource(source);
        nmsg->setDestination(destination);
        nmsg->setType(type);
        nmsg->setKind(color);
        return nmsg;

    }


    void Node::heartBeat(){

        cancelEvent(CALL_HEART_BEAT_MSG);

        if(leaderId == id){


            changeColorAndIcon((char*)"blue",(char*)"device/mainframe_vl");

            cont_to_generate_msg = cont_to_generate_msg +1;
            int i;
            for(i = 0; i< numbersOfGates; i++){
                    HEARTBEAT_MSG  = createMsg(HEART_BEAT, HEART_BEAT_ENUM, id, i, MAGENTA);
                    send(HEARTBEAT_MSG, "out",i);
            }

            scheduleAt(simTime()+0.5, CALL_HEART_BEAT_MSG);
            if(cont_to_generate_msg == 3){
                destination = rand()%numbersOfGates;
                HANDLER_MSG = createMsg(HANDLER, HANDLER_ENUM, id, destination,BLUE);
                send(HANDLER_MSG, "out",destination);
                cont_to_generate_msg = 0;
            }
        }
    }
}; // namespace
