let design =
{
    "$schema": "https://vega.github.io/schema/vega-lite/v3.json",
    "data": {
        "name": "dimensions"
    },
    "config": {
        "axis": { "grid": true, "tickBand": "extent" }
    },
    "encoding": {
        "x": {
            "field": "id",
            "title": "Mean of b",
            "timeUnit": "year",
            "axis": {
                "title": "Mean of b",
            }
        }
    },
    "layer": [
        {
            "mark": {
                "type": "errorband",
                "borders": {
                    "opacity": 0.3,
                    "strokeDash": [2, 2]
                }
            },
            "encoding": {
                "y": {
                    "field": "max",
                    "type": "quantitative",
                    "scale": { "zero": false },
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
                    { "field": "mean", "type": "quantitative", "title": "Mean" },
                    { "field": "name", "type": "nominal", "title": "Dimension" },
                ]
            }
        },
        {
            "mark": {
                "type": "line",
                "point": true
            },
            "encoding": {
                "x": {
                    "field": "id",
                    "type": "quantitative"
                },
                "y": {
                    "field": "avg",
                    "type": "quantitative"
                },
                "color": {
                    //"value": "red",
                },
                "tooltip": [
                    { "field": "avg", "type": "quantitative", "title": "Average" },
                ]
            }
        },
        {
            "mark": {
                "type": "line",
                "strokeWidth": 0.5,
                "strokeDash": [6, 4]
            },
            "encoding": {
                "x": {
                    "field": "id",
                    "type": "quantitative",
                    "title": "asddad"
                },
                "y": {
                    "field": "mean",
                },
                "tooltip": [
                    { "field": "mean", "type": "quantitative", "title": "Mean" },
                ]
            }
        }
    ]
}