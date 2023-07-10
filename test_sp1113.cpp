# include <string>
# include <iostream>
# include <vector>
# include <fstream>

using namespace std ;


class ToolList {
	public :
	bool isWhiteSpace(string str) {
		if ( str == " ")
			return true ;
		else if ( str == "\n" )
			return true ;
		else if ( str == "\t" )
			return true ;
		else
			return false ;
	} // isWhiteSpace()

	bool isInteger( string str ) {
		int str_length = str.length() ;
		for ( int i = 0 ; i < str_length ; i++ ) {
			if ( '0' > str.at( i ) || str.at( i ) > '9' ) 
				return false ;
		} // for
		return true ;
	} // isInteger
	
	string LowerToUpper( string str ) {
		string dealed ;
		char temp ;
		for ( int i = 0 ; i < str.length() ; i++ ) {
			temp = str.at( i ) ;
			if (temp >= 'a' && temp <= 'z') {
				temp = temp - 32 ;
			} // if
			dealed = dealed + temp ;
		} // for
		return dealed ;
	} // LowerToUpper()
	
	string UpperToLower( string str ) {
		string dealed ;
		char temp ;
		for ( int i = 0 ; i < str.length() ; i++ ) {
			temp = str.at( i ) ;
			if (temp >= 'A' && temp <= 'Z') {
				temp = temp + 32 ;
			} // if
			dealed = dealed + temp ;
		} // for
		return dealed ;
	} // UpperToLower()
		
};

class Assembler {
	
	struct Form {
		string loc;
		string sCode; // source code
		string op;
		string mCode; // object code
		Form * next = NULL ;
	} ;	

	struct Lexial {
		int Type ;
		int Value ;
		Lexial * next = NULL ;
	} ;	
	
	struct Token {
		string source_code ;
		Lexial * link = NULL ;
		
		Token() {}
		Token( string str ) {	 source_code = str ;	} 
	};
	
	struct hashtable {
		string contents ;
		bool exists ;
	} ;
	
	int tablesize[8] ;        // 設8個  只用1~7 
	hashtable * table[8] ;
	
	string fileName ;
	fstream aFile ;  // Pointer to File 
	
	vector<string> buffer ;   // store Token    ready to deal 
	ToolList tool ;           // Some tools that really practical
	vector<Token> all_information ;
	vector<Form> forms;
	
	string startLoc ;
	
// table_one(1) ;   // instruction
// table_two(2) ;   // pseudo word
// table_three(3) ; // register
// table_four(4) ; // delimiter
// table_five(5) ;  // label or symbol
// table_six(6) ; // integer real number
// table_seven(7) ; // string 

	public : 
	
	
	void Init() {
		for ( int i = 0 ; i < 8 ; i++ ) {
			table[i] = NULL ;
		}
		buffer.clear() ;
		all_information.clear() ;
		forms.clear() ;
	} // Init()
	
	
	void OpenFileName() {  // 讀檔名 、 開檔 
		
		do {
			cout << "Please Enter Input File Name (ex. SIC_input.txt) : " ;
			//cout << "Now Default : SIC_input.txt" << endl ;;
			cin >> fileName ;
			//fileName = "SIC_input.txt" ;
			aFile.open( fileName.c_str(), fstream::in) ;	
			
			if ( aFile.is_open() ) {
				cout << "Open " <<  fileName << " Successed" << endl << endl; 
			} // if
			else
				cout << "Open " <<  fileName << " Error      :(" << endl ; 
		} while ( !aFile.is_open() ) ;
	} // OpenFileName() 
	
		
	void CloseFile() {   // 關檔 
		aFile.close() ;
	} // CloseFile()
	
		
	void WriteFile() {
		string outputname = fileName + "_output.txt" ;
		aFile.open( outputname.c_str(), fstream::out) ;	
		
		
		aFile << "Line\tLocation\tSource code\t\t\t\t\t\tObject code\n" ;
		aFile << "-------------------------------------------------------------------------------------\n";

		for ( int i = 0, line = 5 ; i < all_information.size() ; i++, line+=5 ) {
			aFile << line << "\t" ;
			aFile << all_information.at( i ).source_code ;
			if ( all_information.at( i ).link != NULL ) {
				Lexial * walk = all_information.at( i ).link;
				while ( walk != NULL ) {
					aFile << "(" << walk->Type << "," << walk->Value << ") "  ;
					walk = walk->next ;
				} // while	
			} // if
			aFile << endl ;
		} // for
		cout << endl << "Analyze " << fileName << "  Successed!" << endl ;
		aFile.close() ;
		
	} // WriteFile()


	vector<string> ReadTable( string tableName ) {    // 讀Table1 || Table2 || Table3 || Table4
		string temp ;
		char ch ;
		fstream tablefile ;
		vector<string> table_input ;
		tablefile.open( tableName.c_str(), fstream::in) ;
		if ( !tablefile.is_open() ) {
			cout << "Table Open :" << tableName << "Failed! "<< endl ;
			cout << "請關閉視窗重新再來一次。  :(" << endl ;
		} // if 
		while( ! tablefile.eof() ) {
			tablefile >> temp ;
			table_input.push_back(temp) ;
		} // while
		tablefile.close() ;
		return table_input ;
	} // ReadTable
	
	
	void CreateHashTable( int index ) {   // 根據index 建造該HashTable 
		table[index] = new hashtable[tablesize[index]] ;
	} // CreateHashTable
	
	
	void InitHashTable(int instr) {    // num 為 table 的編號 
		// 呼叫 ReadTable()  CreateHashTable() 
		string fileName ; 
		vector <string> table_input ;  // 讀預設的input 
		for( int i = 1 ; i <= 7 ; i++ ) {
			table_input.clear() ;
			if ( instr == 1 ) {
				if ( i == 1 )
					fileName = "Table1.table" ;	
				if ( i == 2 )
					fileName = "Table2.table" ;	
				if ( i == 3 )
					fileName = "Table3.table" ;	
				if ( i == 4 )
					fileName = "Table4.table" ;	
			} // if
			else {
				if ( i <= 4 ) {
					cout << "請輸入Table" << i << "檔名 : " ;
					cin >> fileName ; 		
				} // if
			} // else
			
			if ( i <= 4 ) {
				table_input = ReadTable( fileName ) ;
				tablesize[i] = table_input.size() + 1 ;
			} // if
			else 
				tablesize[i] = 100 ;
				
			CreateHashTable( i ) ; 
			if ( i <= 4 ) {
				for( int j = 0 ; j < tablesize[i]-1 ; j++ ) {
					table[i][j+1].contents = table_input[j] ;
					table[i][j+1].exists = true ;
				} // for			
			} // if
			else {
				for ( int j = 0 ; j < tablesize[i]-1 ; j++ ) {
					table[i][j+1].contents = "\0" ;
					table[i][j+1].exists = false ;
				} // for
			} // else	
		} // for
	} // InitHashTable
	

	int isTable( int index, string str ) {     // 判斷是不是Table[index]存在 ， 若是的話， 回傳該index ， 否則回傳-1 
		for ( int i = 1 ; i < tablesize[index] ; i++ ) {
			if ( table[index][i].exists == true ) {
				if ( str == table[index][i].contents ) 
					return i ;				
			} // if
		} // for
		
		return -1 ;
	} // isTable()

	
	int countHash( string str ) {     // 計算str內的hash值  ( mod=100 ) 
		int sum = 0 ; 
		for ( int i = 0 ; i < str.length() ; i++ ) {
			sum = sum + (int)str.at( i ) ;
		} // for
		return sum % 100 ;
		
	} // countHash()

	
	int inputHash( int index, int hashNum, string str ) {   //  塞入Table[index] 值 ( table5 ~ table8 限定)  // return table_index ;    
	  // warning : table index 0 = NULL     from [0,99] 
	  	int hashIndex ;
	
	  	hashIndex = hashNum ;
	  	
		while(true) {
			
			if ( table[index][hashIndex].exists == false ) {
				table[index][hashIndex].contents = str ;
				table[index][hashIndex].exists = true ;
				break ;
			} // if
			else {
				if ( hashIndex + 1 > 99 )	  
					hashIndex = 0 ;
				else
					hashIndex = hashIndex + 1 ;
			} // else
		} // while
		return hashIndex ;
	} // inputHash()




	void GetToken() {     // 切Token 主程式
		char onechar ;
		string chToString = "\0" ;
		string temp = "\0";  // lots of onechar ;
		string line = "\0" ; // store the whole line   
		
		int tokenlist = 0 ;
		int tokenType ; 
		int tokenValue ;
		
		bool exception_semicolon = false ;   // 分號 Read Comment
		bool exception_apostrophe = false ; ; // 單引號 Read 
		while( !aFile.eof() ) {
			aFile.get( onechar ) ;
			chToString = onechar ;
			
			if( tool.isWhiteSpace(chToString) ) 	{	// 碰到WhiteSpace 
				line = line + chToString ;	
				
				if ( temp != "\0" && exception_semicolon == false && exception_apostrophe == false) {     // 確保temp有東西才塞入buffer    (不會亂塞 \0 
					buffer.push_back(temp) ;
					temp = "\0" ; 
				} // if	
				
				
				if ( chToString == "\n" ) {
					buffer.push_back( "\n" ) ;
					all_information.push_back( Token(line) ) ;
					line = "\0" ; 
					exception_semicolon = false ;     // reset exception_semicolon exception_apostrophe
					exception_apostrophe = false ;   
				} // if ( end line )
				else if ( exception_apostrophe )
					temp = temp + chToString ;
			} // if(WhiteSpace)
			
			else if ( exception_semicolon ) {
				line = line + chToString ;
			} // 分號處理 
			else if ( isTable(4, chToString) != -1 ) {	// --判斷Table 
				if ( temp != "\0" ) {
					buffer.push_back(temp) ;
					temp = "\0" ;
				} // if
				buffer.push_back(chToString) ;
				
				line = line + chToString ; 
				
				if ( chToString == "." )        // ReadComment
					exception_semicolon = true ;
				else if ( chToString == "\'" ) 
					exception_apostrophe = ! exception_apostrophe ;
			

			} // else if(Delimeter)
			else if ( !aFile.eof() ) {
				temp = temp + chToString ;
				line = line + chToString ;
			} // else
		} // while
		
		
		if ( temp != "\0" ) {    // 處理最後一行不是Enter結尾 
			all_information.push_back( Token(line+"\n") ) ;
			buffer.push_back( temp ) ;
			buffer.push_back ("\n" ) ; 
		} // if 

	} // GetToken()



	void CheckTable() {

		
		// 要主動去撈 buffer 內的資料 (切完的Token) 
		//在此要更新其他的linked-list串接  大雜燴都在此完成
		string temp ;
		int tokenType ;
		int tokenValue ;
		Lexial * walk ;
		bool exception_toTable6 = false ;
		bool exception_toTable7 = false ;
		bool exception_dealed = false ;
	
		int count_index = 0 ;


		while( buffer.size()!= 0 ) {

			exception_dealed = false ;       // reset ;
			temp = buffer.front() ;
			buffer.erase( buffer.begin() ) ;
			
	
			if ( temp == "\n" ) {            // check exception
				exception_toTable6 = false ;
				exception_toTable7 = false ;
				exception_dealed = true ;
				count_index++ ;
			} // if
			else if ( ( tool.LowerToUpper(temp) == "C" || tool.LowerToUpper(temp) == "X" ) && buffer.size() != 0 && exception_toTable6 == false && exception_toTable7 == false ) {             // check exception
				if ( buffer.front() == "\'" ) {
					if ( temp == "X" ) 
						exception_toTable6 = true ;
					else
						exception_toTable7 = true ; 
					exception_dealed = true ;
				} // if
			} // else if
			if ( exception_dealed == false ) { 
				tokenValue = isTable( 1, tool.UpperToLower(temp) ) ;
				if ( tokenValue != -1 ) {                // this area is table1 
					tokenType = 1 ;
				} // if
				else {
					tokenValue = isTable( 2, temp ) ;
					if( tokenValue != -1 ) {         	// this area is table2  
						tokenType = 2 ;
					} // if
					else {
						tokenValue = isTable( 3, tool.LowerToUpper(temp) ) ;
						if ( tokenValue != -1 ) {    // this area is table3
							tokenType = 3 ;
						} // if
						else {
							tokenValue = isTable( 4, temp ) ;
							if ( tokenValue != -1 ) { // this area is table4 
								tokenType = 4 ;
							} // if
							else {
								if ( tool.isInteger(temp) || exception_toTable6 ) {  // this area is table6(Interger)
									tokenType = 6 ;
									tokenValue = isTable( 6, temp ) ;
									if ( tokenValue == -1 ) {
										int hashNum = countHash( temp ) ;
										tokenValue = inputHash( 6, hashNum, temp ) ;
									} // if
									exception_toTable6 = false ; 
								} // if
								else if ( exception_toTable7 ) {	// this area is table7( String )
									tokenType = 7 ;
									tokenValue = isTable( 7, temp ) ;
									if ( tokenValue == -1 ) {
										int hashNum = countHash( temp ) ;
										tokenValue = inputHash( 7, hashNum, temp ) ;
									} // if
									exception_toTable7 = false ;  // reset
								} // else if
								else {                                // this area is table5(Symbol/Label)
									tokenType = 5 ;
									tokenValue = isTable( 5, temp ) ;
									if ( tokenValue == -1 ) {
										int hashNum = countHash( temp ) ;
										tokenValue = inputHash( 5, hashNum, temp ) ;
									} // if
								} // else (only table5)
							} // else (not table4)
						} // else (not table3)
					} // else (not table2)
				} // else (not table1)		
				
				if ( all_information.at( count_index ).link == NULL ) {
					all_information.at( count_index ).link = new Lexial() ;
					walk = all_information.at( count_index ).link ;
				} // if
				else {
					walk->next = new Lexial() ;
					walk = walk->next ;
				} // else
				walk->Type = tokenType ;
				walk->Value = tokenValue ;		                                   
			} // if (not exception)
		} // while
	} // CheckTable()
		
	string findStartLoc() {
		bool foundStart = false;
		string startLoc = "";

		for (const auto& token : all_information) {
			if (token.link != nullptr) {
				Lexial* lexial = token.link;
				if (lexial->Type == 1 && table[1][lexial->Value].contents == "START") {
					Lexial* operandLexial = lexial->next;
					if (operandLexial != nullptr && operandLexial->Type == 6) {
						startLoc = table[6][operandLexial->Value].contents;
						foundStart = true;
						break;
					}
				}
			}
		}

		if (!foundStart) {
			startLoc = "0001";
		}

		return startLoc;
	}

	void run() {
		OpenFileName() ;
		GetToken() ;
		CheckTable() ;

		string startLoc = findStartLoc(); 
		cout << startLoc << endl;
		
		CloseFile() ;
		WriteFile() ;
				
	
	} // run()

	
} ;


int main() {
	int instr ;
	Assembler assembler;
	assembler.Init() ;	
	while(1) {
		do {
			cout << "*******  Lexical Analysis  *******" << endl ;
			cout << "***** 1 . 直接輸入檔名       *****" << endl ;
			cout << "***** 2 . 客製Table1~Table4  *****" << endl ;
			cout << "**********************************" << endl ;		
			cout << endl ;
			cout << "請輸入指令 ([1,2]) : " ;
			cin >> instr ;
			if ( instr != 1 && instr != 2 )
				cout << "Error! Please Try Again!" << endl ;
		} while ( instr != 1 && instr != 2);
	
		assembler.InitHashTable(instr) ;
		assembler.run();
	} // while

} // main()
