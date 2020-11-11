#pragma once

#include "RdtSender.h"

class TCPSender :public RdtSender
{
private:
	int base, nextSeq, curPkt, count;
	Packet window[windowSize << 1];
	int tag[windowSize << 1];
	bool waitingState;

public:
	bool send(const Message& message);
	bool getWaitingState();
	void receive(const Packet& packet);
	void timeoutHandler(int seqNum);
	void showWindow(int op);

public:
	TCPSender();
	~TCPSender();
};