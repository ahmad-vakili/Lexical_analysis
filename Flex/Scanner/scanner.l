%{
#include <stdio.h>
#include <string.h>

int row = 1;
int col = 1;

void update_col(const char* yytext) {
    col += strlen(yytext);
}
%}

%%

"start"|"finish"|"then"|"if"|"repeat"|"var"|"int"|"float"|"do"|"read"|"print"|"void"|"return"|"dummy"|"program" {
    printf("(KEYWORD,\"%s\",row=%d,col=%d)\n", yytext, row, col);
    update_col(yytext);
}

"=="|"!="|"=>"|"=" {
    printf("(RELOP,\"%s\",row=%d,col=%d)\n", yytext, row, col);
    update_col(yytext);
}

[a-zA-Z][a-zA-Z]{0,7} {
    printf("(IDENTIFIER,\"%s\",row=%d,col=%d)\n", yytext, row, col);
    update_col(yytext);
}

[0-9]+(\.[0-9]+)? {
    printf("(NUMBER,\"%s\",row=%d,col=%d)\n", yytext, row, col);
    update_col(yytext);
}

[\n] {
    row++;
    col = 1;
}

[ \t\r]+ {
    col += yyleng;
}

. {
    printf("(ERROR,\"%s\",row=%d,col=%d)\n", yytext, row, col);
    update_col(yytext);
}

%%

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input.abc\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Cannot open input file");
        return 1;
    }

    yyin = file;
    yylex();
    fclose(file);
    return 0;
}