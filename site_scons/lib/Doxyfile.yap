
PROJECT_NAME           = "${PROJECTNAME}"
PROJECT_NUMBER         = "(Version $(REVISION))"

INPUT                  = .
RECURSIVE              = NO
ALPHABETICAL_INDEX     = NO

###########################################################################
# Layout customization

@INCLUDE = "$(LIBDIR)/Doxyfile"

DOXYFILE_ENCODING      = UTF-8
INPUT_ENCODING         = UTF-8

ALWAYS_DETAILED_SEC    = YES
MULTILINE_CPP_IS_BRIEF = YES
DETAILS_AT_TOP         = YES

BUILTIN_STL_SUPPORT    = YES

SUBGROUPING            = NO
EXTRACT_ALL            = YES
HIDE_FRIEND_COMPOUNDS  = YES
INTERNAL_DOCS          = YES

FILE_PATTERNS          = *.c \
                         *.cc \
                         *.cci \
                         *.ct \
                         *.cti \
                         *.h \
                         *.hh \
                         *.ih \
                         *.mpp \
                         *.dox
EXCLUDE_PATTERNS       = *.test.cc \
                         *.test.hh \
                         .* \
                         *~ \
                         "#*#"
EXCLUDE_SYMBOLS        = boost std
IMAGE_PATH             = .

SOURCE_BROWSER         = YES
COLS_IN_ALPHA_INDEX    = 3

GENERATE_LATEX         = NO
GENERATE_XML           = NO
GENERATE_MAN           = NO

MACRO_EXPANSION        = YES
EXPAND_ONLY_PREDEF     = YES

EXTERNAL_GROUPS        = NO

HAVE_DOT               = YES
CLASS_GRAPH            = YES
COLLABORATION_GRAPH    = NO
GROUP_GRAPHS           = NO
GRAPHICAL_HIERARCHY    = NO
DIRECTORY_GRAPH        = NO
DOT_GRAPH_MAX_NODES    = 10
MAX_DOT_GRAPH_DEPTH    = 5
DOT_MULTI_TARGETS      = YES
DOT_CLEANUP            = NO

SEARCHENGINE           = NO

LAYOUT_FILE            = $(LIBDIR)/DoxygenLayout.xml
HTML_HEADER            = $(TOPDIR)/$(output_dir)/doxyheader.html
HTML_FOOTER            = $(TOPDIR)/$(output_dir)/doxyfooter.html
