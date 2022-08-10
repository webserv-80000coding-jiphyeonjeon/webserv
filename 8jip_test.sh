#!/bin/bash
# ANCHOR 테스트 항목 템플릿
# echo -e "\033[32;1m"Test Title"\033[m"
# body
# echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR 열려있는 포트 정보 확인하기
echo -e "\033[32;1m"Check Port"\033[m"
lsof -nP -i | grep LISTEN | grep webserv
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR GET: index 가져오기
echo -e "\033[32;1m""GET: default page(index.html)""\033[m"
curl -X GET http://localhost:9999 -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR GET: 폴더 파도타기
echo -e "\033[32;1m""GET: another file""\033[m"
curl -X GET http://localhost:9999/YoupiBanane/Yeah/not_happy.bad_extension -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR GET: error page (404 - default error page)
echo -e "\033[32;1m""GET: Error page""\033[m"
curl -X GET http://localhost:9999/unknown-file -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR GET: 집현전 로고 가져오기
echo -e "\033[32;1m""GET: image(png)""\033[m"
curl -X GET http://localhost:4242/logo --output test_output.png -v
echo -e "✅ Check image from ./test_output.png"
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR GET: books폴더 autoindex 확인
echo -e "\033[32;1m""GET: autoindex 42jiphyeonjeon books ""\033[m"
curl -X GET http://localhost:4242/books -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a


# ANCHOR POST: 임시로 사용할 파일 생성
echo -e "\033[32;1m""POST: create temp_post""\033[m"
curl -X POST -H "Content-Type:  plain/text" --data $'Please delete using DELETE method\n' http://localhost:9999/free_to_post/temp_post -v
echo -e "✅ Check file from ./www/temp_post"
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR POST: 생성한 파일에 추가하기
echo -e "\033[32;1m""POST: append temp_post""\033[m"
curl -X POST -H "Content-Type:  plain/text" --data $'This is append message\n' http://localhost:9999/free_to_post/temp_post -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR POST: client body 제한
echo -e "\033[32;1m""POST: client_body_buffer_size error""\033[m"
curl -X POST -H "Content-Type:  plain/text" --data $'0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n' http://localhost:9999/free_to_post/temp_post -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR POST: 80000coding 폴더에 포스팅하기
echo -e "\033[32;1m""POST: post to 80000coding""\033[m"
curl -X POST -H "Content-Type:  plain/text" --data $'dha: 내 서브도 받아보시지' --resolve 80000coding:8000:127.0.0.1 http://80000coding:8000/post_by_dha -v
echo -e "✅ Check file from ./www/80000coding/post_by_dha"
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR POST: 포스팅에 추가
echo -e "\033[32;1m""POST: append to 80000coding""\033[m"
curl -X POST -H "Content-Type:  plain/text" --data $'\nhyojekim: 🎾🏸🏓🏐' --resolve 80000coding:8000:127.0.0.1 http://80000coding:8000/post_by_dha -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR POST: CGI 실행
echo -e "\033[32;1m""POST with cgi: cgi_test will make this message to upper case!""\033[m"
curl -X POST -H "Content-Type:  plain/text" --data $'cgi_test will make this message to upper case!' http://localhost:9999/subject_cgi/temp_cgi.foo -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a


# ANCHOR DELETE: 임시로 생성한 파일 삭제
echo -e "\033[32;1m"DELETE"\033[m"
curl -X DELETE http://localhost:9999/free_to_post/temp_post -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR DELETE: 허용되지 않은 경우 (limit_except)
echo -e "\033[32;1m"DELETE"\033[m"
curl -X DELETE http://localhost:8000/80000coding/post_by_dha -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a


# ANCHOR 같은 포트이지만 server_name이 다른 경우 비교
echo -e "\033[32;1m"Same port \&\& Different server_name"\033[m"
echo -e "\033[0;33m"localhost"\033[m"
curl -X GET http://localhost:4242 -v
echo -e "\n"=================================================================="\n"
echo -e "\033[0;33m"jiphyeonjeon"\033[m"
curl -X GET --resolve jiphyeonjeon:4242:127.0.0.1 http://jiphyeonjeon:4242 -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a


# ANCHOR return을 이용한 redirection
echo -e "\033[32;1m""Redirect (return)""\033[m"
curl -X GET http://localhost:8000/url -v
echo -e "\n"=================================================================="\n"
curl -X GET http://localhost:4242/url -v
echo -e "⬇️  press any key to continue ⬇️ "; read $a

# ANCHOR 부하테스트
echo -e "\033[32;1m""Siege Test: quit with ^C""\033[m"
siege -R <(echo connection: keep-alive) -c100 http://localhost:9999 -b
echo -e "⬇️  press any key to continue ⬇️ "; read $a

echo -e "\033[32;1m""Cleaning 🧹""\033[m"
rm test_output.png;
rm www/80000coding/post_by_dha;
echo -e "⬇️  press any key to continue ⬇️ "; read $a

echo  "█████████████████████████████████████                                        "
echo  "██                                                                           "
echo  "██                                                                           "
echo  "██                         ███                                               "
echo  "██  ███████████████████    ███                                               "
echo  "██      ███      ███       ████████    ██████████████████████                "
echo  "██      ███      ███       ████████       █████████████████████              "
echo  "██  ████████████████████   ███                               ████            "
echo  "██                         ███                                  ██           "
echo  "██    ██████████████████████████                                  ███        "
echo  "██                          ████                                    ██"
echo  "██    ██████████████████████████                                      ███    "
echo  "██    ████                              ████████████ ████████████         ███"
echo  "██    ██████████████████████████        █████████    ████████████            "
echo  "██                                      ██████       ████████████            "
echo  "██                                      ██           ████████████            "
echo  "██                                                  ████████████             "
echo  "██                                               ████████████                "
echo  "██                                            █████████████                  "
echo  "██     ████████████████████████           █████████████                      "
echo  "██                         ████         █████████████           █            "
echo  "██     ████████████████████████         █████████████        ████            "
echo  "██               ███       ████         █████████████     ███████            "
echo  "██               ███       ████         █████████████  ██████████            "
echo  "██               ███                                                         "
echo  "██               ███                                     "
echo  "██   █████████████████████████████                                           "
echo  "██                                                                           "
echo  "██                                                                           "
echo  "██                            dha & hyojekim                                 "

echo -e "\033[32;1m""\n\n\t🎉 Finish!! Thank you for evaluating :)""\033[m"