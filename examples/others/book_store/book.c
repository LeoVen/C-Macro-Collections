// Separated file for all book functions
typedef struct book
{
    int isbn;
    char *name;
} book;

book *new_book(int isbn, const char *name)
{
    book *b = malloc(sizeof(book));
    b->isbn = isbn;
    b->name = strdup(name);
    return b;
}
