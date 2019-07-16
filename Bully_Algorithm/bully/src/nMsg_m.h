//
// Generated file, do not edit! Created by nedtool 5.4 from nMsg.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __NMSG_M_H
#define __NMSG_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>nMsg.msg:12</tt> by nedtool.
 * <pre>
 * message nMsg
 * {
 *     int source;
 *     int destination;
 *     int type;
 *     string tag;
 *     bool hartBeat;
 *     int leaderId;
 * 
 * }
 * </pre>
 */
class nMsg : public ::omnetpp::cMessage
{
  protected:
    int source;
    int destination;
    int type;
    ::omnetpp::opp_string tag;
    bool hartBeat;
    int leaderId;

  private:
    void copy(const nMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const nMsg&);

  public:
    nMsg(const char *name=nullptr, short kind=0);
    nMsg(const nMsg& other);
    virtual ~nMsg();
    nMsg& operator=(const nMsg& other);
    virtual nMsg *dup() const override {return new nMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSource() const;
    virtual void setSource(int source);
    virtual int getDestination() const;
    virtual void setDestination(int destination);
    virtual int getType() const;
    virtual void setType(int type);
    virtual const char * getTag() const;
    virtual void setTag(const char * tag);
    virtual bool getHartBeat() const;
    virtual void setHartBeat(bool hartBeat);
    virtual int getLeaderId() const;
    virtual void setLeaderId(int leaderId);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const nMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, nMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef __NMSG_M_H

