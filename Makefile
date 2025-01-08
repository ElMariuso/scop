# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mthiry <mthiry@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/04 21:14:36 by mthiry            #+#    #+#              #
#    Updated: 2024/12/29 18:32:11 by mthiry           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -Iinclude -Idependencies
LDFLAGS := -lglfw -ldl

SRC_DIR := src
OBJ_DIR := obj
DEP_DIR := dependencies

SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
GLAD_FILES := $(DEP_DIR)/glad/glad.c
TARGET := scop

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(GLAD_FILES) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re