#第一题
"""
1.
with open('input.txt', 'r') as file:
    text = file.read()
    sentences = text.split('. ')  # 使用句号加空格进行分句

for sentence in sentences:
    print(sentence.strip('.')+'.')
"""

""" 
2.
from collections import Counter

with open('input.txt', 'r') as file:
    text = file.read().lower()  # 将文本转换为小写

# 去除文本中的标点符号和特殊字符
text = ''.join(c if c.isalnum() or c.isspace() else ' ' for c in text)

# 将文本拆分为单词列表
words = text.split()

# 使用Counter统计单词出现次数
word_counts = Counter(words)

# 输出结果
for word, count in word_counts.items():
    print(f"{word}: {count}")
"""

"""
3.
from collections import Counter

with open('input.txt', 'r') as file:
    text = file.read().lower()

# 去除文本中的标点符号和特殊字符
text = ''.join(c if c.isalnum() or c.isspace() else ' ' for c in text)

# 将文本拆分为单词列表
words = text.split()

# 使用Counter统计单词出现次数
word_counts = Counter(words)

# 筛选出现次数大于等于2的单词
filtered_words = {word: count for word, count in word_counts.items() if count >= 2}

# 按照出现次数降序排序
sorted_words = sorted(filtered_words.items(), key=lambda x: x[1], reverse=True)

# 写入结果到out.txt
with open('out.txt', 'w') as out_file:
    for word, count in sorted_words:
        out_file.write(f"{word}: {count}\n")
"""
 
"""
4.
with open('input.txt', 'r') as file:
    text = file.read().lower()

# 去除文本中的标点符号和特殊字符
text = ''.join(c if c.isalnum() or c.isspace() else ' ' for c in text)

# 将文本拆分为句子列表
sentences = text.split('. ')

# 初始化位置字典
word_positions = {}

# 统计单词位置信息
for index, sentence in enumerate(sentences):
    words = sentence.split()
    for position, word in enumerate(words, 1):
        if word not in word_positions:
            word_positions[word] = []
        word_positions[word].append((index + 1, position))

# 写入结果到position.txt
with open('position.txt', 'w',encoding='utf-8') as position_file:
    for word, positions in word_positions.items():
        position_file.write(f"{word}: ")
        position_file.write(', '.join(f"第 {sentence} 个句子第 {position} 个单词" for sentence, position in positions))
        position_file.write(".\n")
"""

#第二题

"""
1.
def draw_parallelogram(Mode, height):
    if Mode == 'right':
        for i in range(height):
            print(' ' * i + '*' * 18)
    elif Mode == 'left':
        for i in range(height):
            print(' ' * (height - i - 1) + '*' * 18)
    else:
        print("Invalid Mode. Please choose 'right' or 'left'.")


# 测试函数
draw_parallelogram('right', 5)
print()  # 空行
draw_parallelogram('left', 5)

""" 

""" 
2.
def save_draw_graph(Mode, height):
    if Mode == 'right':
        filename = 'right_parallelogram.txt'
    elif Mode == 'left':
        filename = 'left_parallelogram.txt'
    else:
        print("Invalid Mode. Please choose 'right' or 'left'.")
        return

    with open(filename, 'w',encoding='utf-8') as file:
        if Mode == 'right':
            for i in range(height):
                line = ' ' * i + '*' * 18
                print(line)
                file.write(line + '\n')
        else:
            for i in range(height):
                line = ' ' * (height - i - 1) + '*' * 18
                print(line)
                file.write(line + '\n')

    print(f"Pattern saved to {filename}.")


# 测试函数
save_draw_graph('right', 5)
print()  # 空行
save_draw_graph('left', 5)
""" 

#第三题

#第一题---------------------
books_info = []

with open('books.txt', 'r') as file:
    for line in file:
        line = line.strip()
        if line:
            book_data = line.split('\t')
            book_info = {
                '书名': book_data[0],
                '作者': book_data[1],
                'ISBN编号': book_data[2],
                '库存数量': int(book_data[3])
            }
            books_info.append(book_info)

# 打印图书信息
for book in books_info:
    print(book)

#------------------------第二题
def add_book_info(books_info, title, author, ISBN, stock):
    for book in books_info:
        if book['书名'] == title and book['作者'] == author and book['ISBN编号'] == ISBN:
            print("系统中已存在此书")
            return

    new_book = {
        '书名': title,
        '作者': author,
        'ISBN编号': ISBN,
        '库存数量': stock
    }
    books_info.append(new_book)
    print("图书信息已成功添加")


# 测试函数

add_book_info(books_info, 'snow country', 'Yasunari Kawabata', '008X', 3)  # 图书信息已成功添加
print(books_info)

#--------------------------第三题
def search_books(books_info, author):
    matching_books = []
    for book in books_info:
        if book['作者'] == author:
            matching_books.append(book)

    sorted_books = sorted(matching_books, key=lambda x: len(x['书名']))

    for book in sorted_books:
        print(f"书名: {book['书名']}, 作者: {book['作者']}, ISBN编号: {book['ISBN编号']}, 库存数量: {book['库存数量']}")



search_books(books_info, 'Haruki Murakami')  # 查询图书


#-------------------第四题
def borrow_book(books_info, ISBN):
    for book in books_info:
        if book['ISBN编号'] == ISBN:
            if book['库存数量'] > 0:
                book['库存数量'] -= 1
                print("借阅成功")
            else:
                print("库存不足")
            break

    with open('books_update.txt', 'w') as file:
        for book in books_info:
            file.write(f"{book['书名']}\t{book['作者']}\t{book['ISBN编号']}\t{book['库存数量']}\n")



borrow_book(books_info, '402X')  # 借阅图书