#ifndef __PORTERSETMMING__H
#define __PORTERSTEMMING__H



class PorterStemming{
	
	public:
	
		PorterStemming(){}
		
		
		int cons(int i);
		
		int m();
		
		int vowelinstem();
		
		int doublec(int j);
		
		int cvc(int i);
		
		int ends(char * s);
		
		void setto(char *s);
		
		void r(char * s);
		
		void step1ab();
		
		void step1c();
		
		void step2();
		
		void step3();
		
		void step4();
		
		void step5();
		
		int stem(char *p, int i, int j);
			
};

#endif
