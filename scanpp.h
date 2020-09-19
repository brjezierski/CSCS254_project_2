typedef enum {
	t_read, 
	t_write, 
	t_id, 
	t_literal, 
	t_gets,
	t_add, 
	t_sub, 
	t_mul, 
	t_div, 
	t_lparen, 
	t_rparen, 
	t_eof
} token;


const int MAX_TOKEN_LEN=128;

token scan();

extern "C" char token_image[MAX_TOKEN_LEN];