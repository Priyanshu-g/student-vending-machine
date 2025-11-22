_Monitor Bank {
  public:
	Bank( unsigned int numStudents );
	// id is 0 indexed!
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};