import json


passes = ['name']

def sanitize_value(value):
    """将值置为空值（根据类型）"""
    if isinstance(value, str):
        return ""
    elif isinstance(value, (int, float)):
        return 0
    elif isinstance(value, bool):
        return False
    elif isinstance(value, list):
        return []
    elif isinstance(value, dict):
        return {}
    else:
        return None


def sanitize_config(data):
    """递归遍历 JSON 数据并置空所有值"""
    if isinstance(data, dict):
        return {k: sanitize_config(v) for k, v in data.items()}
    elif isinstance(data, list):
        return [sanitize_config(item) for item in data]
    else:
        return sanitize_value(data)

def select_value(key, value):
    if key in passes:
        return value
    """将值置为空值（根据类型）"""
    if isinstance(value, str):
        return ""
    elif isinstance(value, (int, float)):
        return 0
    elif isinstance(value, bool):
        return False
    elif isinstance(value, list):
        return []
    elif isinstance(value, dict):
        return {}
    else:
        return None

def trans_config(key, data):
    """递归遍历 JSON 数据并置空所有值"""
    if isinstance(data, dict):
        return {k: trans_config(k, v) for k, v in data.items()}
    elif isinstance(data, list):
        return [trans_config(None, item) for item in data]
    else:
        return select_value(key, data)


# 读取原始配置
with open('config/config.json', 'r') as f:
    config = json.load(f)

# 置空所有值
sanitized_config = trans_config(None, config)

# 写入 example 文件
with open('config/config.example.json', 'w') as f:
    json.dump(sanitized_config, f, indent=2, ensure_ascii=False)