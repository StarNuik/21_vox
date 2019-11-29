NAME = vox.out

# Our files
SRCS = $(wildcard srcs/*.cpp)
SRCS += $(wildcatd srcs/**/*.cpp)
INCL = -I includes/ -I ~/.brew/include/

# Imgui files
SRCS += $(wildcard imgui/*.cpp)
SRCS += imgui/examples/imgui_impl_glfw.cpp imgui/examples/imgui_impl_opengl3.cpp
INCL += -I imgui/ -I imgui/examples/

# Additional settings
FLAGS = -Wall -Wextra -Werror
LIB = -lm -L ~/.brew/lib/ -lglfw -lglew
FRAMEWORK = -framework OpenGL
OBJS = $(SRCS:.cpp=.o)
DEPENDS = $(patsubst %.cpp, %.d, $(SRCS))

all: binclean $(OBJS) $(NAME)
	@echo "\033[92mCompilation successful!\033[39m"

-include $(DEPENDS)

%.o: %.cpp
	@g++ $(FLAGS) -MMD -MP -c $< -o $@ $(INCL)

$(NAME): $(OBJS)
	@g++ $(FLAGS) -o $(NAME) $(LIB) $(FRAMEWORK) $(OBJS)

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPENDS)

fclean: clean
	@rm -f $(NAME)

binclean:
	@rm -f $(NAME)

re: fclean all