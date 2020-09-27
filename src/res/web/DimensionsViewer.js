let design =
{
    "$schema": "https://vega.github.io/schema/vega-lite/v3.json",
    "data": {
        "name": "dimensions"
    },
    "layer": [
        {
            "mark": {
                "type": "errorband",
                "borders": {
                    "opacity": 0.5,
                    "strokeDash": [6, 4]
                }
            },
            "encoding": {
                "y": {
                    "field": "max",
                    "type": "quantitative",
                    //"scale": { "zero": false },
                    "title": "Point value"
                },
                "y2": { "field": "min" },
                "x": {
                    "field": "id",
                    
                },
                "tooltip": [
                    { "field": "min", "type": "quantitative", "title": "Minimum" },
                    { "field": "max", "type": "quantitative", "title": "Maximum" },
                    { "field": "avg", "type": "quantitative", "title": "Average" },
                    { "field": "name", "type": "quantitative", "title": "Dimension name" },
                ]
            }
        },
        {
            "mark": {
                "type": "line",
                "point": true
            },
            "encoding": {
                "y": {
                    "field": "avg",
                    "type": "quantitative"
                },
                "x": {
                    "field": "id",
                }
            }
        }
    ]
}