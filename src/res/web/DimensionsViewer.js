
let design = {
    "$schema": "https://vega.github.io/schema/vega-lite/v4.json",
    "width": "container",
    "height": "container",
    "autosize":
    {
        "type": "fit",
        "resize": true,
    },
    "signals": [
        {
            "name": "colourSelector",
            "bind": { "input": "select", "options": ["black", "steelblue", "red"] },
            "value": "red"
        }
    ],
    "data": {
        "name": "dimensions",
    },
    "layer": []
}

function getRangeBand(channelIndex) {
    return {
        "mark": {
            "type": "errorband",
            "borders": {
                "opacity": 0.5,
                "strokeDash": [3, 3]
            }
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channelIndex
                }
            }
        ],
        "encoding": {
            "x": {
                "field": "dim",
                "type": "ordinal"
            },
            "y": {
                "field": "min",
                "type": "quantitative"
            },
            "y2": {
                "field": "max",
                "type": "quantitative"
            },
        }
    }
}

function getLineMark(channelIndex, field, opacity, strokeWidth, strokeDash) {
    return {
        "mark": {
            "type": "line",
            "strokeWidth": strokeWidth,
            "strokeDash": strokeDash,
            "opacity": opacity
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channelIndex
                }
            }
        ],
        "encoding": {
            "x": {
                "field": "dim",
                "type": "nominal"
            },
            "y": {
                "field": field,
                "type": "quantitative",
            },
            "color": {
                //"signal": `color${channelIndex}`
                "value": {
                    "signal": "colourSelector"
                }
            }
        }
    }
}

function getColorSignal(channelIndex) {
    return {
        "name": `color${channelIndex}`,
        "value": "#ffffff",
    }
}

function addChannel(design, channelIndex) {
    design.signals.push(getColorSignal(channelIndex));
    design.layer.push(getRangeBand(channelIndex));
    design.layer.push(getLineMark(channelIndex, "v0", 1, 1, []));
}