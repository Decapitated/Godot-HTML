@tool
extends HtmlRect

func _on_dom_ready(url):
    print("DOM ready")
    print("URL: ", url)
    return {
        "bool": true,
        "int": 69,
        "float": 3.1415,
        "string": "Hello World!",
        "null": null,
        "callable": test_callable,
        "vector3": Vector3(1, 2, 3),
        "plane": Plane(1, 2, 3, 4),
        "quaternion": Quaternion(1, 2, 3, 4),
        "dictionary": {
            "test": "Hello World!",
        }
    }

func test_callable():
    print("test_callable")