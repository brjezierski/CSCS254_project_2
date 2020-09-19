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
	t_eof,
	t_if,
	t_end,
	t_while,
	t_eq,
	t_neq,
	t_lt,
	t_gt,
	t_lte,
	t_gte
} token;


const int MAX_TOKEN_LEN=128;

token scan();

extern "C" char token_image[MAX_TOKEN_LEN];