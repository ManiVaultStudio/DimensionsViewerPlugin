
let design = {
    "$schema": "https://vega.github.io/schema/vega-lite/v4.json",
    "width": "container",
    "height": "container",
    "autosize":
    {
        "type": "fit",
        "resize": true,
    },
    "signals": [],
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
                "opacity": 0.3,
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
            "color": {
                "value": {
                    "signal": `color${channelIndex}`
                }
            }
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
                "value": {
                    "signal": `color${channelIndex}`
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
    design.layer.push(getRangeBand(channelIndex));
    design.layer.push(getLineMark(channelIndex, "v0", 1, 1, []));
}