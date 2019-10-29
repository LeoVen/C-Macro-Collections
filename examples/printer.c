#include <cmc/list.h>
#include <utl/log.h>

#define TAB_SIZE 4
#define ROW_INI_CAP DOC_LETTERS_IN_ROW
#define DOC_LETTERS_IN_ROW 100
#define DOC_ROWS_IN_PAGE 50
#define DOC_MARGIN 4

CMC_GENERATE_LIST(row, row, char)
CMC_GENERATE_LIST(doc, document, struct row *)

void row_deallocator(struct row *r)
{
    row_free(r, NULL);
}

struct document *document_load(const char *file_name);
void document_empty_line(FILE *fptr);
void document_page_outline(FILE *fptr, int outline);
void document_page_margin(FILE *fptr, int margin);
void document_print(struct document *doc, FILE *fptr);

struct document *document_load(const char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (!file)
    {
        cmc_log_error("File %s not found\n", file_name);
        return NULL;
    }

    struct row *initial_row = row_new(ROW_INI_CAP);

    struct document *doc = doc_new(100);
    doc_push_back(doc, initial_row);

    int ch;

    while ((ch = getc(file)) != EOF)
    {
        char c = (char)ch;

        struct row *current_row = doc_back(doc);

        if (c == '\n')
        {
            struct row *new_row = row_new(ROW_INI_CAP);
            doc_push_back(doc, new_row);
        }
        else if (c == '\t')
        {
            intmax_t left = TAB_SIZE;

            while (left > 0 && row_count(current_row) < DOC_LETTERS_IN_ROW)
            {
                row_push_back(current_row, ' ');
                left--;
            }

            if (left > 0)
            {
                struct row *new_row = row_new(ROW_INI_CAP);
                doc_push_back(doc, new_row);

                while (left > 0)
                {
                    row_push_back(new_row, ' ');
                    left--;
                }
            }
        }
        else if (DOC_LETTERS_IN_ROW == row_count(current_row))
        {
            struct row *new_row = row_new(ROW_INI_CAP);
            row_push_back(new_row, c);
            doc_push_back(doc, new_row);
        }
        else
            row_push_back(current_row, c);
    }

    return doc;
}

void document_empty_line(FILE *fptr)
{
    fputs("|", fptr);
    for (size_t i = 0; i < DOC_LETTERS_IN_ROW + DOC_MARGIN * 2; i++)
    {
        fputc(' ', fptr);
    }
    fputs("|\n", fptr);
}

void document_page_outline(FILE *fptr, int outline /* 0 - header, 1 - footer */)
{
    if (outline == 1)
    {
        document_empty_line(fptr);
#ifdef DOC_PAGE_NUMBER
        static intmax_t page = 1;

        fputs("|", fptr);
        for (size_t i = 0; i < (DOC_LETTERS_IN_ROW + DOC_MARGIN) - 8; i++)
        {
            fputc(' ', fptr);
        }

        fprintf(fptr, "%8" PRIdMAX "", page);
        page++;

        document_page_margin(fptr, 1);
        document_empty_line(fptr);
#endif
    }

    fputs("+", fptr);
    for (size_t i = 0; i < DOC_LETTERS_IN_ROW + DOC_MARGIN * 2; i++)
    {
        fputc('-', fptr);
    }
    fputs("+\n", fptr);

    if (outline == 0)
        document_empty_line(fptr);
}

void document_page_margin(FILE *fptr, int margin /* 0 - first margin, 1 - second margin */)
{
    if (margin == 0)
        fputs("|", fptr);

    for (size_t i = 0; i < DOC_MARGIN; i++)
    {
        fputc(' ', fptr);
    }

    if (margin == 1)
        fputs("|\n", fptr);
}

void document_print(struct document *doc, FILE *fptr)
{
    fputc('\n', fptr);

    intmax_t current_page_row = 0, total_rows = doc_count(doc);

    // Print document
    for (intmax_t i = 0; i < total_rows; i++, current_page_row++)
    {
        if (current_page_row == 0)
            document_page_outline(fptr, 0);

        struct row *current_row = doc_get(doc, i);

        if (!current_row)
            cmc_log_fatal("Unexpected NULL pointer for %s", "current_row");

        document_page_margin(fptr, 0);

        for (size_t k = 0; k < row_count(current_row); k++)
            fputc(row_get(current_row, k), fptr);

        // Trailing margin
        for (size_t k = 0; k < DOC_LETTERS_IN_ROW - row_count(current_row); k++)
            fputc(' ', fptr);

        document_page_margin(fptr, 1);

        // Print the end of the page and reset
        if (current_page_row == DOC_ROWS_IN_PAGE - 1)
        {
            document_page_outline(fptr, 1);
            current_page_row = -1;
            fputc('\n', fptr);
        }
    }

    // Print trailing page
    if (total_rows % DOC_ROWS_IN_PAGE != 0)
    {
        for (size_t i = 0; i < DOC_ROWS_IN_PAGE - current_page_row; i++)
            document_empty_line(fptr);

        document_page_outline(fptr, 1);
    }

    fputc('\n', fptr);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        cmc_log_error("Please define one argument only for %s", argv[0]);
        return 1;
    }

    struct document *doc = document_load(argv[1]);

    document_print(doc, stdout);

    doc_free(doc, row_deallocator);

    return 0;
}
