# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    testit.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/15 15:07:51 by rhallste          #+#    #+#              #
#    Updated: 2018/02/15 15:10:21 by rhallste         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cat /dev/urandom | head -c 50 > test/input.txt
base64 -i test/input.txt -o test/output.txt
./ft_ssl base64 -i test/input.txt -o test/my_output.txt
diff test/output.txt test/my_output.txt
