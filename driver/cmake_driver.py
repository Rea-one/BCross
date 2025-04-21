import json

with open("config/config.json", "r") as f:
    config = json.load(f)

# 正确提取path列表
paths = config["path"]

with open("config.cmake", "w") as f:
    for item in paths:
        # 将名称转为大写并处理特殊字符（如连字符转下划线）
        var_name = item["name"].upper().replace("-", "_")  
        path_value = item["path"]
        # 使用f-string格式化，并为路径添加双引号
        f.write(f'set({var_name}_ROOT "{path_value}")\n')