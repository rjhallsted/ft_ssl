# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    testit.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/15 15:07:51 by rhallste          #+#    #+#              #
#    Updated: 2018/02/15 15:58:25 by rhallste         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cat /dev/urandom | head -c 50 > test/input.txt
base64 -i test/input.txt -o test/output.txt
./ft_ssl base64 -i test/input.txt -o test/my_output.txt
diff test/output.txt test/my_output.txt
./ft_ssl base64 -d -i test/my_output.txt -o test/my_input.txt
diff test/input.txt test/my_input.txt
