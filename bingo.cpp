#include "bingo.h"

int Carton::print_version(){

     printf("%s",
            "BINGO v1.0 [2014-Set-2] Julián Calvo Murillo <juancalmur@hotmail.com> José Ml Matarrita Campos <josemmatarritacampos@hotmail.es>\n"

            "This is free software distributed under Creative Commons Attribution 4.0\n"
            "International (CC BY 4.0) license.\n");

    return 0;
}

int Carton::print_help(){

    printf("%s",
		    "Create Bingo cards and simulate game sessions. Usage:\n"
			"bingo generate NUMBER card.svg [-s symbols.txt] [-c callingcard.svg]\n"
			"bingo call [card.idx] [-t type] [-s symbols.txt] [-w winners]\n"

			"Actions:\n"
			"generate               Generate random cards in current directory\n"
			"call                   Simulate a bingo game session\n"

			"Options:\n"
			"NUMBER                 Number of cards to be generated\n"
			"card.svg               Graphic design of the card with ${field} placeholders\n"
			"card.idx               Contains all the generated cards and their 24 or 25 symbols\n"
			"-c callingcard.svg     Fill out the given calling card with all 75 symbols\n"
			"-s symbols.txt         Use the given 75 symbols instead of numbers\n"
			"-t type                Pattern to be matched in order to win. Valid types:\n"
									"four-corners, straight-line, roving-L, blackout\n"
			"-w winners             Number of winners until finish the session\n");

	return 0;
}

const char* Carton::generate_str(const char* str){
	
	const char* cardboard = "";
	int multiplo = 1;
	bool finish = false;
	int random_number = 1;
 
	if (str[2] == 'c'){
		cardboard="*";
		return cardboard;
	}

	else if (str[4] == '1'){multiplo = 1;}
 
	else if (str[4] == '2'){multiplo = 2;}
    
    else if (str[4] == '3'){multiplo = 3;}
     
    else if (str[4] == '4'){multiplo = 4;}
      
    else if (str[4] == '5'){multiplo = 5;}
   
	random_number = rand() % (15*multiplo) + 1;
  
	while(finish != true){
		
		if(random_number<(((15*multiplo)-15)))
			random_number = rand() % (15*multiplo) + 1;
		
		else 
			finish = true;
	}
	
	std::string string = "";
	string = string + random_number;
	return cardboard = string.data(); 
}

const char* Carton::generate_str(const char* str, const char* symbols_filename){
	
	const char* cardboard = "";
	int random_symbol = 1;
	std::ifstream symbols(symbols_filename);
	std::vector<std::string> simbolos;
	simbolos.reserve(75);
	
	if (symbols){
      
		std::string sentence;
		
		for (int i = 0; i < 75; i++){
			std::getline(symbols, sentence);
			simbolos.push_back(sentence);
		}
		
		symbols.close();
		
	}
 
	if (str[2] == 'c'){
		cardboard="*";
		return cardboard;
	}
	
	random_symbol = rand() % (75) + 1;
	
	std::string string = simbolos[random_symbol];
	return cardboard = string.data(); 
}

bool Carton::generate_file(std::ifstream& source, const char* temp_filename, std::ofstream& index, size_t cards, bool symbols, const char* symbols_filename){

	std::ofstream target(temp_filename);
	std::vector<std::string> index_vector;
	index_vector.reserve(75);
	std::string line;
	const char* replacement = "";
		
	if (!target){
		std::cerr << "Bingo: could not make file" << target << std::endl;
		return 1;
	}
	
	while (std::getline(source, line)){
		
		size_t pos = line.rfind("${");   		// Asume que solo hay un ${ en line
		
		if (pos != std::string::npos){
			
			size_t f_pos = line.rfind('}') + 1;
			size_t s = f_pos - pos;
			std::string str = line.substr (pos, s); 
			
			if (symbols == false)
				replacement = generate_str(str.data());
			else if (symbols == true)
				replacement = generate_str(str.data(), symbols_filename);
	
			index_vector.push_back(replacement);
			line.replace(pos, s, replacement);
		}
	
		target << line << std::endl; 
		
	}
	
	put_index(index_vector, index, cards);
	
	target.close();
	return true;
}

bool Carton::generate_filename(const char* cards, const char* source_filename, bool symbols, const char* symbols_filename){

	int num = atoi(cards);
	bool result = true;
	std::ifstream source(source_filename);
	std::string symbol = source_filename;
	size_t last = symbol.find_last_of("."); 
	std::string filename = symbol.substr(0, last);
	char index_filename[strlen(source_filename) + 1];
	strcat(strcpy(index_filename, filename.data()), ".idx");
	std::ofstream index(index_filename);
 
	if (!source){
		std::cerr << "Bingo: could not open" << source << std::endl;
		return 1;
	}
 
	for (int i = 1; i <= num; i++){
 
		char temp_int[strlen(cards)];
		sprintf(temp_int,"%i", i);
  
		char temp_filename[strlen(source_filename)+ strlen(temp_int) + 1];
		strcat(strcpy(temp_filename, source_filename), (temp_int));
  
		if (symbols == false)
			result = generate_file(source, temp_filename, index, i);
		else if (symbols == true)
			result = generate_file(source, temp_filename, index, i, symbols, symbols_filename);
		
		source.clear();
        source.seekg(0);
		
	}
 
	source.close();
	index.close();
	return result;
}

bool Carton::replace(const char* symbols_filename, const char* source_filename){

	std::ifstream symbols(symbols_filename);
	std::vector<std::string> simbolos;
	int vector_size = 75;
	simbolos.reserve(vector_size);
	int j = 0;

	if (symbols){
      
		std::string sentence;
		
		for (int i = 0; i < vector_size; i++){
			std::getline(symbols, sentence);
			simbolos.push_back(sentence);
		}
		
		symbols.close();
		
	}
	
	std::ifstream source(source_filename);
	
	if (source) {
	
		std::string symbol =  symbols_filename;
		int last = symbol.find_last_of("."); 
		std::string filename = symbol.substr(0, last); 
	
		char temp_filename[strlen(symbols_filename) + 1 + strlen(source_filename) + 1];
		strcat(strcpy(temp_filename, filename.data()), "-");
		strcat(temp_filename, source_filename);
		
		std::ofstream target(temp_filename);
	
		std::string line;
        
        while (std::getline(source, line)){
		        
			size_t pos = line.rfind("${");
          
			if (pos != std::string::npos){
    
				size_t f_pos = line.rfind('}') + 1;
				size_t s = f_pos - pos;
				std::string str = line.substr (pos, s); 
				line.replace(line.begin() + pos, line.begin() + f_pos, simbolos[j]);
				j++;
   
			}
          
			target << line << std::endl;
    
        }
		
		source.close();
		target.close();
	
	}
	
	return 0;
}

bool Carton::call(const char* index_filename, const char* gametype){

	printf("%s",
		"\nBingo call for symbols 1 through 75\n"
		"Press ENTER key to call next number...?\n\n"

		"B -- -- -- -- -- -- -- -- -- -- -- -- -- -- --\n"
		"I -- -- -- -- -- -- -- -- -- -- -- -- -- -- --\n"
		"N -- -- -- -- -- -- -- -- -- -- -- -- -- -- --\n"
		"G -- -- -- -- -- -- -- -- -- -- -- -- -- -- --\n"
		"O -- -- -- -- -- -- -- -- -- -- -- -- -- -- --\n");
		
	std::vector<int> call_count;
	unsigned vector_size = 75;
	call_count.reserve(vector_size);
	bool full = false;
	unsigned count = 0;
	
	for (unsigned i = 0; i < vector_size; i++)
		call_count[i] = 0;
	
	std::string line;
	
	std::ifstream index(index_filename);
	
	if (!index) {
		std::cerr << "Bingo: could not open" << index << std::endl;
		return 1;
	}
	
	while ((full == false) && (std::getline(std::cin, line))){
		
		srand (time(NULL));
		int random_number = rand() % (vector_size) + 1;
		int call_number = random_number - 1;
	
		if (call_count[call_number] == 0){
			
			call_count[call_number] = random_number;
			count++;
			
			std::cout << "Number: " << random_number << std::endl;
			
			printf("\n");
			
			std::string bng = "BINGO";
			int travel = 0;
	
			for(int i = 0; i < 5; i++) {
	
				std::cout << bng[i] << " ";
	
					for (int j = 0; j < 15; j++){
		
						if (call_count[travel] == 0)
							std::cout << "--" << " ";
				
						else
							std::cout << call_count[travel] << " ";
				
						travel++;
						
					}
		
					printf("\n");
	
			}
			
			printf("\n");
			
		}
			
		else 
			std::cout << "Number " << random_number << " already called." << std::endl;
			
		if (count == vector_size) 
			full = true;
		
		std::string gametype_str = gametype;
			
		bool result = admin_gametype(index, gametype_str, call_count);
		
		if (result)
			return 0;
				
	}
	
	std::cout << "Everything has been called from the card." << std::endl;
	
	index.close();
	return 0;

}

bool Carton::call_symbols(const char* symbols_filename){

	printf("%s",
		"\nBingo call for symbols 1 through 75\n"
		"Press ENTER key to call next number...?\n\n"

		"B               I               N               G               O\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n"
		"--------------- --------------- --------------- --------------- ---------------\n\n");
		
	std::vector<std::string> call_count;
	unsigned size = 75;
	call_count.reserve(size);
	bool full = false;
	unsigned count = 0;
	
	std::ifstream symbols(symbols_filename);
	std::vector<std::string> simbolos;
	simbolos.reserve(size);

	if (symbols){
      
		std::string sentence;
		
		for (int i = 0; i < 75; i++){
			std::getline(symbols, sentence);
			simbolos.push_back(sentence);
		}
		
		symbols.close();
		
	}
	
	for (unsigned i = 0; i < size; i++)
		call_count.push_back("---------------");
	
	std::string line;
	
	while ((full == false) && (std::getline(std::cin, line))){
		
		srand (time(NULL));
		int x = rand() % (size) + 1;
		int y = x - 1;
	
		if (call_count[y] == "---------------"){
			
			call_count[y] = simbolos[y];
			count++;
			
			std::cout << "Symbol " << count << ": " << simbolos[y] << std::endl;
			
			printf("\n");
			
			int travel = 0;
			std::cout <<"B               I               N               G               O\n";
	
			for (int j = 0; j < 14; j++){
						
				for(int i = 0; i < 5; i++) {
						const char* temp =  call_count[travel].data();
						int z = 15 - strlen(temp);
						std::cout << temp;
						
						for (int h = 0; h < z; h++)
							std::cout << "-";
							
						std::cout << " ";
						travel = travel + 15;
					}
		
					printf("\n");
					travel = travel - 75;
					travel++;
	
			}
			
			printf("\n");
			
		}
			
		else 
			std::cout << "Symbol \"" << simbolos[y] << "\" already called." << std::endl;
			
		if (count == size) 
			full = true;
			
	}
	
	std::cout << "Everything has been called from the card." << std::endl;

	return 0;

}

void Carton::put_index(std::vector<std::string>& index_vec, std::ofstream& index, int count){

	std::string line = "";
	int n = 25;
 
	if (index){
 
		char buffer[128];
		sprintf(buffer, "%i", count);
		std::string cardboard = "Cardboard ";
  
		char temp_filename[strlen(cardboard.data()) + strlen(buffer) + 1];
		strcat(strcpy(temp_filename, cardboard.data()), buffer);
 
		index << temp_filename << std::endl;  
 
		for (int i = 0; i < n; ++i){
			line = index_vec[i];
			index << line << std::endl;
		}
 
	}
 
	else 

	std::cerr << "Bingo: could not open" << index << std::endl;
 
}

bool Carton::search_int(int find, std::vector<int>& call_vector){
	
	size_t size = 75;
	
	/*for (int i = 0; i < 75; ++i){
		std::cout << call_vector[i] <<"vec" << std::endl;
	}*/

	for (unsigned i = 0; i < size; ++i) {
		if (find == call_vector[i]){
			std::cout<<"true"<<std::endl;
			return true;
		}	}
	
	
	
	return false;

}

void Carton::announce_winner(std::string& cardboard_check){
	printf("\n");
	std::cout << "B I N G O ! ! !" << std::endl;
	std::cout << "Winner 1:" << cardboard_check << std::endl;
}

bool Carton::blackout(std::ifstream& index, std::vector<int>& vec){

	std::string words;
	std::string cardboard_check;
	std::string dummy;
	int count = 0;
	int compare = 0;
	bool search = true;
	
	while (std::getline(index, words) && (search == true)){

		if (words.front() == 'C')
			cardboard_check = words;
			
		else if (words.front() == '*')
			dummy = words;

		else {
			
			compare = atoi(words.data());
				
			if (search_int(compare, vec) == false){
				search = false;
			}
				
			else if (search_int(compare, vec) == true){
				count++;
			}
				
		}
		
	}
	
	index.clear();
	index.seekg(0);
	
	if (count == 24)
		announce_winner(cardboard_check);
	
	return 0;
	
}

bool Carton::four_corners(std::ifstream& index, std::vector<int>& vec){
	/* nota j son columnas, e i son las filas*/

	std::string line;
	std::string cardboard_check;
	std::string dummy;
	int compare = 0;
	int contador = 0;
	int contador_esquinas = 0;

	while ((getline(index,line)) && (contador_esquinas != 4)){

		if (line.front() == 'C'){
			cardboard_check = line;
			++contador;
		}

		else if (line.front() == '*'){
			dummy = line;
		}
   
		else {
			for ( int j= 1; j < 6; ++j ){
				if( (j==1) || (j==5) ){
					for( int i= 1; i< 6; ++i){

						if( (i==1)|| (i==5)){

							compare = atoi(line.data());
							if(search_int(compare,vec) == true)
							++contador_esquinas;
			
						}
					}
				
				}
			}	
		}
	}
		
	if (contador_esquinas == 4)
		announce_winner(cardboard_check);

   return 0;

}

bool Carton::straight_line(std::ifstream& index, std::vector<int>& vec){
	
	
	 if (Vertical_Line(index, vec)==true){ 
			std::cout<< "You won Vertical"<<std::endl;
		}
		else if (Horizontal_Line(index,  vec)==true){
			std::cout<< "You won Horizontal"<<std::endl;
		}
	
	return 0;

}

bool Carton::Vertical_Line(std::ifstream& index, std::vector<int>& vec){
	std::string line;
	std::string cardboard_check;
	std::string dummy;
	int compare = 0;
	int contador_casillas = 0;

	while ((getline(index,line)) && (contador_casillas != 5)){

		if (line.front() == 'C'){
			cardboard_check = line;
		}
		else{
			for( int j=1;j<6; ++j){
				contador_casillas=0;
				for( int i=1;i<6; ++i){
					compare = atoi(line.data());
					if(search_int(compare,vec) == true){
						++contador_casillas;
					}
					else{
						dummy=line;
					}

				}
			}
		}
	}
	if(contador_casillas==5){
		announce_winner(cardboard_check);
		return true;
	}
	else
		return false;

}

bool Carton::Horizontal_Line(std::ifstream& index, std::vector<int>& vec){
/* i son filas y j son columnas*/
	std::string line;
	std::string cardboard_check;
	std::string dummy;
	int compare = 0;
	int contador_casillas = 0;

	while ((getline(index,line)) && (contador_casillas != 5)){

		if (line.front() == 'C'){
			cardboard_check = line;
		}
		else if (line.front() == '*'){
			dummy = line;
		}
		else{
			for (int i=0; i<6; ++i){

				contador_casillas=0;
				for ( int j=0; j<6; ++j){
					compare = atoi(line.data());
					if(search_int(compare,vec) == true)
						++contador_casillas;
				}
			}
		}
	}
	if(contador_casillas==5){
		announce_winner(cardboard_check);
		return true;
	}
	else
		return false;
}

bool Carton::roving_L(std::ifstream& index, std::vector<int>& vec){
	
	std::string words;
	std::string cardboard_check;
	std::string dummy;
	int count = 0;
	int compare = 0;
	int L1 = 0;
	int L2 = 0;
	int L3 = 0;
	int L4 = 0;
	
	while ((std::getline(index, words)) && (L1 < 4) && (L2 < 4) && (L3 < 4) && (L4 < 4)){

		if (words.front() == 'C')
			cardboard_check = words;
			
		else if (words.front() == '*')
			dummy = words;
		
		else if ((count == 1) || (count == 6) || (count == 11) || (count == 16) || (count == 21) || (count == 22) || (count == 23) || (count == 24) || (count == 25)){
			
			compare = atoi(words.data());
			if (search_int(compare, vec))
				L1++;
			std::cout << "L1 " << L1 << std::endl;
		}
		
		else if ((count == 1) || (count == 2) || (count == 3) || (count == 4) || (count == 5) || (count == 10) || (count == 15) || (count == 20) || (count == 25)){
			
			compare = atoi(words.data());
			if (search_int(compare, vec))
				L2++;
			std::cout << "L2 " << L2 << std::endl;
		}
		
		else if ((count == 21) || (count == 22) || (count == 23) || (count == 24) || (count == 25) || (count == 5) || (count == 10) || (count == 15) || (count == 20)){
			
			compare = atoi(words.data());
			if (search_int(compare, vec))
				L3++;
			std::cout << "L3 " << L3 << std::endl;
		}
		
		else if ((count == 1) || (count == 6) || (count == 11) || (count == 16) || (count == 21) || (count == 2) || (count == 3) || (count == 4) || (count == 5)){
			
			compare = atoi(words.data());
			if (search_int(compare, vec))
				L4++;
			std::cout << "L4 " << L4 << std::endl;
		}
		
		std::cout << "count " << count << std::endl;
		count++;
	
	}
	
	index.clear();
	index.seekg(0);
	
	if ((L1 == 9) || (L2 == 9) || (L3 == 9) || (L4 == 9))
		announce_winner(cardboard_check);
	
	return 0;

}

bool Carton::diagonal(std::ifstream& index, std::vector<int>& vec){
	
	int left = 0;
	int right = 0;
	std::string words;
	std::string cardboard_check;
	std::string dummy;
	int count = 0;
	int compare = 0;
	std::vector<int> diagonal_left;
	std::vector<int> diagonal_right;
	
	while ((std::getline(index, words)) && (left < 4) && (right < 4)){
		
		if (words.front() == 'C')
			cardboard_check = words;
			
		else if (words.front() == '*')
			dummy = words;
		
		else if ((count == 1) || (count == 7) || (count == 19) || (count == 25)){
			
			compare = atoi(words.data());
			if (search_int(compare, vec))
				left++;
		}
		
		else if ((count == 5) || (count == 9) || (count == 17) || (count == 21)){
			
			compare = atoi(words.data());
			if (search_int(compare, vec))
				right++;
		}
			
		count++;
		
	}
	
	index.clear();
	index.seekg(0);
	
	if ((left == 4) || (right == 4))
		announce_winner(cardboard_check);
	
	return 0;

}

bool Carton::admin_gametype(std::ifstream& index, std::string& gametype, std::vector<int>& vec){

	if (gametype.compare("blackout") == 0){										
		return blackout(index, vec);
		}
	
	else if (gametype.compare("four-corners") == 0){
		return four_corners(index, vec);
		}	

	else if (gametype.compare("roving-L") == 0){
		return roving_L(index, vec);
		}

	else if (gametype.compare("straight-line") == 0){
		return straight_line(index, vec);
		}

	return 0;
}

int Carton::analyze_params(int argc, char* argv[]){

	bool symbols = true;
    
	for (int i = 1; i < argc; ++i){
	
		if (strcmp(argv[i], "--version") == 0)
            return print_version();
		
		else if (strcmp(argv[i], "--help") == 0)
			return print_help();
		
		else if ((strcmp(argv[i], "generate") == 0)){
		
			if (i + 3 >= argc)
				generate_filename(argv[i + 1], argv[i + 2]);
				
			else
				generate_filename(argv[i + 1], argv[i + 2], symbols, argv[i + 4]);
		}
		
		else if ((strcmp(argv[i], "-c") == 0)){
				
			if (i + 1 >= argc)
				return fprintf(stderr, "Bingo: parameter not specified\n");

			replace(argv[i - 1], argv[i + 1]);
		}
		
		else if ((strcmp(argv[i], "call") == 0)){
		
			if (i + 2 >= argc)
				call();
				
			if (i + 3 >= argc)
				call_symbols(argv[i + 2]);
				
			else if (i + 4 >= argc)
				call(argv[i + 1], argv[i + 3]);
		}

	}

	return 0;

}

std::string operator+(std::string& string, int num)
{
	char buffer[128];
	sprintf(buffer, "%i", num);
	return string+buffer;
}