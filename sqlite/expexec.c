#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <ctype.h>

static void usage(const char *prg)
{
    fprintf(stderr, "Usage: %s /PATH/TO/DATABASE SQL_STATEMENT\n", prg);
}

// int callback(void *UNUSED, int n_numCols, char **pp_s_colVal, char **pp_s_colName)
// {
//  for(int i=0; i < n_numCols; i++) {
//      fprintf(stdout, "col: %s; val: %s\n", pp_s_colName[i], pp_s_colVal[i] ? pp_s_colVal[i] : "<NULL>");
//  }
//  return SQLITE_OK;
// }

/**
 * Expand the `sqlite3_exec()` statement into its components
 *
 *
 */
int main(int argc, char *argv[])
{
    if(argc != 3) {
        usage(argv[0]);
        exit(1);
    }

    sqlite3 *db;
    char *p_c_errMsg = NULL;
    const char *p_cc_dbname = argv[1];
    const char *p_cc_sqlStmt = argv[2];

    // Open DB
    int n_fnrc = sqlite3_open_v2(p_cc_dbname, &db, (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE), NULL);
    if(n_fnrc != SQLITE_OK) {
        fprintf(stderr, "Error while opening/creating database %s: %s\n", p_cc_dbname, sqlite3_errstr(n_fnrc));
        exit(n_fnrc);
    }

    // Prepare stmt
    sqlite3_stmt *p_sqlite3_pStmt;
    n_fnrc = sqlite3_prepare_v3(db, p_cc_sqlStmt, 1000, 0 /* No prepFlags */, &p_sqlite3_pStmt, NULL /* Discard unused portion of `zSql` */);
    if(n_fnrc != SQLITE_OK) {
        fprintf(stderr, "Error %d while compiling statement ``%s``: %s\n", n_fnrc, p_cc_sqlStmt, sqlite3_errstr(n_fnrc));
        exit(n_fnrc);
    }

    /*
     * - Keep `step`-ing through the statement until it is done.
     * - For each row, output:
     *   - Column name
     *   - Column data
     */
    n_fnrc = sqlite3_step(p_sqlite3_pStmt);
    while( (n_fnrc == SQLITE_ROW) )
        {
            int n_colsReturned = sqlite3_column_count(p_sqlite3_pStmt);
            // for( int i = 0; i < n_colsReturned; i++) {
            //  callback(NULL, n_colsReturned, sqlite3_column_text(p_sqlite3_pStmt, i), sqlite3_column_name(p_sqlite3_pStmt, i));
            // }
            int n_sqlite3_colType;
            for(int n_col = 0; n_col < n_colsReturned; n_col++) {
                // get,print column name
                const char *pp_c_colName = sqlite3_column_name(p_sqlite3_pStmt, n_col);
                fprintf(stdout, "%16s: ", pp_c_colName);
                // get column type
                n_sqlite3_colType = sqlite3_column_type(p_sqlite3_pStmt, n_col);
                switch (n_sqlite3_colType) {
                    // print column value as appropriate
                case SQLITE_INTEGER:
                    fprintf(stdout, "%d\n", sqlite3_column_int(p_sqlite3_pStmt, n_col));
                    break;
                case SQLITE_FLOAT:
                    fprintf(stdout, "%f\n", sqlite3_column_double(p_sqlite3_pStmt, n_col));
                    break;
                case SQLITE_TEXT: {
                    // fprintf(stdout, "%s\n", sqlite3_column_text(p_sqlite3_pStmt, n_col));
                    const unsigned char *px = sqlite3_column_text(p_sqlite3_pStmt, n_col);
                    for( int j=0; j<sqlite3_column_bytes(p_sqlite3_pStmt, n_col); ++j) {
	                    char c = *(px+j);
	                    // Print the HEX code of unprintable characters
	                    if( !isprint(c) ) { fprintf(stdout, "(0x%x)", c); }
	                    else { putc(c, stdout); }
                    }
                    fprintf(stdout, "\n");
                }
                    break;
                case SQLITE_BLOB:
                    fprintf(stdout, "%p\n", sqlite3_column_blob(p_sqlite3_pStmt, n_col));
                    break;
                case SQLITE_NULL:
                default:
                    // fprintf(stdout, "%s\n", sqlite3_column_(p_sqlite3_pStmt, n_col));
                    fprintf(stdout, "NULL\n");
                    break;
                }
            }
            // Newline separator between rows
            fprintf(stdout, "\n");

            // Step to next row (if any)
            n_fnrc = sqlite3_step(p_sqlite3_pStmt);
            // Loop while more rows
        }

    // Should be done with statement
    if(n_fnrc != SQLITE_DONE) {
        fprintf(stderr, "Error %d while executing statement %s: %s\n", n_fnrc, p_cc_sqlStmt, sqlite3_errstr(n_fnrc));
        exit(n_fnrc);
    }

    // Finalize statement
    n_fnrc = sqlite3_finalize(p_sqlite3_pStmt);
    if(n_fnrc != SQLITE_OK) {
        fprintf(stderr, "Error %d while finalizing statement %s: %s\n", n_fnrc, p_cc_sqlStmt, sqlite3_errstr(n_fnrc));
        exit(n_fnrc);
    }

    // Close DB
    n_fnrc = sqlite3_close_v2(db);
    if(n_fnrc != SQLITE_OK) {
        fprintf(stderr, "Error %d while closing database %s: %s\n", n_fnrc, p_cc_dbname, sqlite3_errstr(n_fnrc));
        exit(n_fnrc);
    }

    exit(0);
}
