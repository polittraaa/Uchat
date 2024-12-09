# Compiler and Flags
CC      = clang
CFLAGS  = -std=c11 -Wall -Wextra -Werror -Wpedantic
NAME	= database
# Directories
SRCDIR  = src
INCDIR  = inc
OBJDIR  = obj
DBDIR   = db

# Libraries
LIBMX       = $(INCDIR)/libmx/libmx.a
SQLITE      = $(INCDIR)/sqlite/sqlite.a

# Include Paths
LIBMX_INC   = $(INCDIR)/libmx/inc
SQLITE_INC  = $(INCDIR)/sqlite/inc
DB_HEADER   = $(INCDIR)/db.h

# BASE64
BASE64_DIR := $(INCDIR)/base64
BASE64_INCDIR := $(BASE64_DIR)/inc
BASE64 := $(BASE64_DIR)/libbase64.a

# Source and Object Files
SRCFILES    = $(wildcard $(SRCDIR)/*.c)
OBJFILES    = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))

# Build Target
all: $(NAME)

# Link the final executable
$(NAME): $(LIBMX) $(SQLITE) $(OBJFILES)
	@echo "Linking $(NAME)..."
	$(CC) $(OBJFILES) -o $(NAME) $(CFLAGS) -I$(INCDIR) -I$(LIBMX_INC) -I$(SQLITE_INC) -I$(DBDIR) -L$(INCDIR)/libmx -L$(INCDIR)/sqlite -lmx -lsqlite3

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBMX_INC) -I$(SQLITE_INC) -I$(DBDIR) -c $< -o $@

# Create the object directory
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Build the libmx library
$(LIBMX):
	@$(MAKE) -C $(INCDIR)/libmx

# Build the sqlite library
$(SQLITE):
	@$(MAKE) -C $(INCDIR)/sqlite

$(BASE64):
	@$(MAKE) -C $(BASE64_DIR)

# Clean object files and executables
clean:
	rm -rf $(OBJDIR)
	rm -f $(NAME)
	@$(MAKE) clean -C $(INCDIR)/libmx
	@$(MAKE) clean -C $(INCDIR)/sqlite
	@$(MAKE) clean -C $(BASE64_DIR)
	rm -f messanger.db
	
# Uninstall libraries and clean everything
uninstall: clean
	@echo "Uninstalling libraries..."
	@$(MAKE) uninstall -C $(INCDIR)/libmx
	@$(MAKE) uninstall -C $(INCDIR)/sqlite


# Rebuild everything from scratch
reinstall: uninstall all
