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
    }

func test_callable():
    print("test_callable")