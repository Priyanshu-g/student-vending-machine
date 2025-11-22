_Task WATCardOffice {
	struct Job {							// marshalled arguments and return future
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
	};
	_Task Courier { ... };					// communicates with bank (YOU DEFINE "Courier")

	void main();
  public:
	_Exception Lost {};						// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
	Job * requestWork() __attribute__(( warn_unused_result ));
};