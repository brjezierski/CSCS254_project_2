
//enums with corresponding numbers as interpreted by C++
typedef enum {
	t_read=0, 
	t_write=1, 
	t_id=2, 
	t_literal=3, 
	t_gets=4,
	t_add=5, 
	t_sub=6, 
	t_mul=7, 
	t_div=8, 
	t_lparen=9, 
	t_rparen=10, 
	t_eof=11,
	t_if=12,
	t_end=13,
	t_while=14,
	t_eq=15,
	t_neq=16,
	t_lt=17,
	t_gt=18,
	t_lte=19,
	t_gte=20
} token;


const int MAX_TOKEN_LEN=128;
const std::ifstream inFile ("test2");

token scan();
void converter(token input_token);
//uses C++ extern function to link the char array in the different code files
extern "C++" char token_image[MAX_TOKEN_LEN];

struct Node;