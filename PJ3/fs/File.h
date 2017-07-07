
class File {
public:
    File(char * file_name); // para ser chamado apenas pelo FileSystem
    ~File(); // deve fechar o arquivo

    int read(int file, char * buf, size_t bytes); // lê no máximo 'bytes' bytes do arquivo e armazena em 'buf'
    int write(int file, char * buf, size_t bytes); // escreve 'bytes' bytes de 'buf' para o arquivo
    int size(char * file_name); // retorna o tamanho do arquivo
    int lseek(int filedes, int offset); // move o apontador do arquivo de acordo com o parâmetro offset (offset pode ser negativo)
    char *nome;
};
