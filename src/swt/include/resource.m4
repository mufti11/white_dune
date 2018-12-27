define(`M4_COMMENT',)
M4_COMMENT( 
 The file resource.m4 is free software; the Free Software Foundation
 gives unlimited permission to copy, distribute and modify it.
)

changecom(//)

define(`M4_FOR_IN',`ifelse($#,2,`$1'(``$2''),`$1'(`$2')`M4_FOR_IN(`$1',shift(shift($@)))')')
define(`M4_FOR_2',`ifelse($#,3,`$1'(``$2'',``$3''),`$1'(`$2',`$3')`M4_FOR_2(`$1',shift(shift(shift($@))))')')

define(`M4_ARG1',`$1'``,'')
define(`M4_ARG2',`$2'``,'')

define(`M4_CHOMP_LAST_COMMA',`ifelse($#,2,`$1',`$1'``,''`M4_CHOMP_LAST_COMMA(shift($@))')')

define(`M4_REVERSE', `ifelse($#, 0, , $#, 1, ``$1'',`M4_REVERSE(shift($@)), `$1'')')

define(`M4_TEMP',M4_FOR_2(`M4_ARG2',M4_LANGUAGES_CONFIG))

define(`M4_LANGUAGES_REV',M4_CHOMP_LAST_COMMA(M4_TEMP))

define(`M4_LANGUAGES',`M4_REVERSE(M4_LANGUAGES_REV)')

define(`M4_TEMP',M4_FOR_2(`M4_ARG1',M4_LANGUAGES_CONFIG))

define(`M4_TEMP',M4_CHOMP_LAST_COMMA(M4_TEMP))

define(`M4_LANGUAGES_ISO3166',`M4_TEMP')

M4_COMMENT(special reverting "for i in" type loop)
define(`M4_FOR_IN_REV_',`ifelse($#,4,`$1'(``$2'',``''),
`M4_FOR_IN_REV_(`$1',`$2',shift(shift(shift($@))))')'
`$1'(`$2',`$3_'))

define(`M4_IDD_INC',`M4_IDD_NUM' `define(`M4_IDD_NUM',incr(M4_IDD_NUM))')
define(`M4_IDD_LANG', ```#define''' IDD_$2$1 ``M4_IDD_INC'')
define(`M4_IDD',M4_FOR_IN_REV_(`M4_IDD_LANG',$1,M4_LANGUAGES_ISO3166,))

define(`M4_IDR_LANG', ```#define''' IDR_$2$1 ``M4_IDD_INC'')
define(`M4_IDR',M4_FOR_IN_REV_(`M4_IDR_LANG',$1,M4_LANGUAGES_ISO3166,))

define(`M4_ID_INC',`M4_ID_NUM' `define(`M4_ID_NUM',incr(M4_ID_NUM))')
define(`M4_ID_LANG', ```#define''' ID_$2$1 ``M4_ID_INC'')
define(`M4_ID',M4_FOR_IN_REV_(`M4_ID_LANG',$1,M4_LANGUAGES_ISO3166,))

define(`M4_IDS_LANG', ```#define''' IDS_$2$1 ``M4_ID_INC'')
define(`M4_IDS',M4_FOR_IN_REV_(`M4_IDS_LANG',$1,M4_LANGUAGES_ISO3166,))

define(`M4_ID_DUNE',`M4_ID(DUNE_'$1`)')

define(`M4_LANG_INC',`M4_LANG_NUM' `define(`M4_LANG_NUM',incr(M4_LANG_NUM))')
