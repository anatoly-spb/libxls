/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright 2004 Komarov Valery
 * Copyright 2006 Christophe Leitienne
 * Copyright 2008-2017 David Hoerl
 * Copyright 2013 Bob Colbert
 * Copyright 2013-2018 Evan Miller
 *
 * This file is part of libxls -- A multiplatform, C/C++ library for parsing
 * Excel(TM) files.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "xls.h"

#include "files.h"

int main(int argc, char *argv[])
{

    xlsWorkBook* pWB;
    xlsWorkSheet* pWS;
    FILE *f;
    unsigned int i;

    xls_error_t code = LIBXLS_OK;
    struct st_row_data* row;
    WORD t,tt;
    char path[2048];
    snprintf(path, sizeof(path), XLS_TEST_FILE);
    pWB=xls_open_file(path, "UTF-8", &code);

    if (pWB == NULL) {
        fprintf(stderr, "Unable to open %s: %s\n", path, xls_getError(code));
        return 1;
    }

    f=fopen ("test.htm", "w");
    for (i=0;i<pWB->sheets.count;i++)
        printf("Sheet N%i (%s) pos %i\n",i,pWB->sheets.sheet[i].name,pWB->sheets.sheet[i].filepos);

    pWS=xls_getWorkSheet(pWB,0);
    if ((code = xls_parseWorkSheet(pWS)) != LIBXLS_OK) {
        fprintf(stderr, "Error parsing worksheet: %s\n", xls_getError(code));
        return 1;
    }

    fprintf(f,"<style type=\"text/css\">\n%s</style>\n",xls_getCSS(pWB));
    fprintf(f,"<table border=0 cellspacing=0 cellpadding=2>");

    for (t=0;t<=pWS->rows.lastrow;t++)
    {
        row=&pWS->rows.row[t];
        //		xls_showROW(row->row);
        fprintf(f,"<tr>");
        for (tt=0;tt<=pWS->rows.lastcol;tt++)
        {
            if (!row->cells.cell[tt].isHidden)
            {
                fprintf(f,"<td");
                if (row->cells.cell[tt].colspan)
                    fprintf(f," colspan=%i",row->cells.cell[tt].colspan);
                //				if (t==0) fprintf(f," width=%i",row->cells.cell[tt].width/35);
                if (row->cells.cell[tt].rowspan)
                    fprintf(f," rowspan=%i",row->cells.cell[tt].rowspan);
                fprintf(f," class=xf%i",row->cells.cell[tt].xf);
                fprintf(f,">");
                if (row->cells.cell[tt].str!=NULL && row->cells.cell[tt].str[0]!='\0')
                    fprintf(f,"%s",row->cells.cell[tt].str);
                else
                    fprintf(f,"%s","&nbsp;");
                fprintf(f,"</td>");
            }
        }
        fprintf(f,"</tr>\n");
    }
    fprintf(f,"</table>");
    printf("Count of rows: %i\n",pWS->rows.lastrow);
    printf("Max col: %i\n",pWS->rows.lastcol);
    printf("Count of sheets: %i\n",pWB->sheets.count);

    fclose(f);
    xls_showBookInfo(pWB);

    return 0;
}
